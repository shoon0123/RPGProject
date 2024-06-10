// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TargetingComponent.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UTargetingComponent::UTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetCollision();
}

void UTargetingComponent::SetTargetableDistance(float Distance)
{
	TargetableDistance = Distance;
	SetSphereRadius(Distance);
}

TObjectPtr<AActor> UTargetingComponent::FindTarget()
{
	TObjectPtr<AActor> TargetActor;

	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController()))
	{
		FVector2D ScreenPosition;
		const FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
		long MinimumDistance = LONG_MAX;

		for (TObjectPtr<AActor> TargetableActor : TargetableActors)
		{
			ScreenPosition = FVector2D::ZeroVector;
			if (UGameplayStatics::ProjectWorldToScreen(PlayerController, TargetableActor->GetActorLocation(), ScreenPosition))
			{
				const double Distance = FVector2D::Distance(ScreenPosition, ViewportSize * 0.5f);
				if (Distance < MinimumDistance)
				{
					MinimumDistance = Distance;
					TargetActor = TargetableActor;
				}
			}
		}
	}
	

	return TargetActor;
}

void UTargetingComponent::SetTarget(TObjectPtr<AActor> Actor)
{
	Target = Actor;
}

void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	SetTargetableDistance(TargetableDistance);
	InitializeTargetableActors();

	OnComponentBeginOverlap.AddDynamic(this, &UTargetingComponent::OnSphereBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UTargetingComponent::OnSphereEndOverlap);
}

void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (TObjectPtr<AActor> TargetableActor : TargetableActors)
	{
		DrawDebugSphere(GetWorld(), TargetableActor->GetActorLocation(), 30.f, 20, FColor::Red, false);
	}

	if (IsValid(Target))
	{
		if (TObjectPtr<AController> Controller = Cast<APawn>(GetOwner())->GetController())
		{
			const FRotator PlayerControllerRotator = Controller->GetControlRotation();
			const FVector CameraLocation = Cast<APlayerCharacter>(GetOwner())->GetCameraLocation();
			const FVector TargetLocation = Target->GetActorLocation();
			
			
			// 2개의 위치 벡터를 입력하여 2번 째 인자의 위치 벡터를 바라보는 방향정보를 반환합니다.
			const FRotator LookAtTarget = UKismetMathLibrary::FindLookAtRotation(CameraLocation, TargetLocation);
			// 1번 째 인자에서 2번 째 인자로 회전 보간합니다.
			const FRotator RInterpToRotator = FMath::RInterpTo(PlayerControllerRotator, LookAtTarget,
				GetWorld()->GetDeltaSeconds(), InterpSpeed);


			Controller->SetControlRotation(RInterpToRotator);
		}
	}
}

void UTargetingComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AddTargetableActor(OtherActor);
}

void UTargetingComponent::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	RemoveTargetableActor(OtherActor);
}

void UTargetingComponent::AddTargetableActor(TObjectPtr<AActor> Actor)
{
	if (IsTargetable(Actor))
	{
		TargetableActors.Add(Actor);
	}

}

void UTargetingComponent::RemoveTargetableActor(TObjectPtr<AActor> Actor)
{
	if (IsTargetable(Actor))
	{
		TargetableActors.Remove(Actor);
	}
}

void UTargetingComponent::InitializeTargetableActors()
{
	TArray<FHitResult> HitResults;
	bool bIsHit = GetWorld()->SweepMultiByChannel(HitResults, GetComponentLocation(), GetComponentLocation(), FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(TargetableDistance));
	if (bIsHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AddTargetableActor(Hit.GetActor());
		}
	}
}

bool UTargetingComponent::IsTargetable(TObjectPtr<AActor> Actor)
{
	return Actor->ActorHasTag(FName("Enemy"));
}

void UTargetingComponent::SetCollision()
{
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}
