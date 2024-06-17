// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TargetingComponent.h"
#include "Character/PlayerCharacter.h"
#include "Player/MyPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UTargetingComponent::UTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetupCollision();
}

void UTargetingComponent::ExecuteLockOn()
{
	if (TObjectPtr<AActor> NewTarget = FindTarget())
	{
		DrawDebugSphere(GetWorld(), NewTarget->GetActorLocation(), 100.f, 20, FColor::Blue, false, 1.f);
		if (!bIsLockOn)
		{
			SetTarget(NewTarget);
			PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
			PlayerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = true;
			bIsLockOn = true;
			if (TObjectPtr<AMyPlayerController> PlayerController = Cast<AMyPlayerController>(PlayerCharacter->GetController()))
			{
				PlayerController->SetLockOnState(true);
			}
		}
	}
}

void UTargetingComponent::CancelLockOn()
{
	if (bIsLockOn)
	{
		SetTarget(nullptr);
		PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		PlayerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = false;
		bIsLockOn = false;
		if (TObjectPtr<AMyPlayerController> PlayerController = Cast<AMyPlayerController>(PlayerCharacter->GetController()))
		{
			PlayerController->SetLockOnState(false);
		}
	}
}

void UTargetingComponent::ChangeLockOnTarget(const FVector2D InputVector)
{
	TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());
	const float TimeSinceLastChangeTarget = GetWorld()->GetRealTimeSeconds() - LastTimeSetTarget;

	if (InputVector.Length() > ChangeTargetSensitivity && TimeSinceLastChangeTarget > ChangeTargetCooldown && PlayerController)
	{
		TObjectPtr<AActor> TargetActor = nullptr;
		FVector2D ScreenPosition = FVector2D::ZeroVector;
		const FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
		long MinimumDistance = LONG_MAX;
		 
		if (FMath::Abs(InputVector.X) >= FMath::Abs(InputVector.Y))
		{
			if (InputVector.X > 0)
			{
				for (TObjectPtr<AActor> TargetableActor : TargetableActors)
				{
					if (TargetableActor != Target && UGameplayStatics::ProjectWorldToScreen(PlayerController, TargetableActor->GetActorLocation(), ScreenPosition))
					{
						const double Distance = ScreenPosition.X - ViewportSize.X * 0.5;

						if (Distance > 0 && Distance < MinimumDistance)
						{
							MinimumDistance = Distance;
							TargetActor = TargetableActor;
						}
					}
				}
			}
			else
			{
				for (TObjectPtr<AActor> TargetableActor : TargetableActors)
				{
					if (TargetableActor != Target && UGameplayStatics::ProjectWorldToScreen(PlayerController, TargetableActor->GetActorLocation(), ScreenPosition))
					{
						const double Distance = ViewportSize.X * 0.5 - ScreenPosition.X;

						if (Distance > 0 && Distance < MinimumDistance)
						{
							MinimumDistance = Distance;
							TargetActor = TargetableActor;
						}
					}
				}
			}
		}
		else
		{
			if (InputVector.Y < 0)
			{
				for (TObjectPtr<AActor> TargetableActor : TargetableActors)
				{
					if (TargetableActor != Target && UGameplayStatics::ProjectWorldToScreen(PlayerController, TargetableActor->GetActorLocation(), ScreenPosition))
					{
						const double Distance = ScreenPosition.Y - ViewportSize.Y * 0.5;

						if (Distance > 0 && Distance < MinimumDistance)
						{
							MinimumDistance = Distance;
							TargetActor = TargetableActor;
						}
					}
				}
			}
			else
			{
				for (TObjectPtr<AActor> TargetableActor : TargetableActors)
				{
					if (TargetableActor != Target && UGameplayStatics::ProjectWorldToScreen(PlayerController, TargetableActor->GetActorLocation(), ScreenPosition))
					{
						const double Distance = ViewportSize.Y * 0.5 - ScreenPosition.Y;

						if (Distance > 0 && Distance < MinimumDistance)
						{
							MinimumDistance = Distance;
							TargetActor = TargetableActor;
						}
					}
				}
			}
		}

		if (IsValid(TargetActor))
		{
			SetTarget(TargetActor);
		}
	}
}


bool UTargetingComponent::IsLockOn()
{
	return bIsLockOn;
}

void UTargetingComponent::SetTargetableDistance(float Distance)
{
	TargetableDistance = Distance;
	SetSphereRadius(Distance);
}

TObjectPtr<AActor> UTargetingComponent::FindTarget()
{
	TObjectPtr<AActor> TargetActor = nullptr;

	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(PlayerCharacter->GetController()))
	{
		FVector2D ScreenPosition = FVector2D::ZeroVector;
		const FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
		long MinimumDistance = LONG_MAX;

		for (TObjectPtr<AActor> TargetableActor : TargetableActors)
		{
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
	LastTimeSetTarget = GetWorld()->GetRealTimeSeconds();
}

void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
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

	UpdateCamera();

}

void UTargetingComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AddTargetableActor(OtherActor);
}

void UTargetingComponent::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	RemoveTargetableActor(OtherActor);
	if (OtherActor == Target)
	{
		CancelLockOn();
	}
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

void UTargetingComponent::SetupCollision()
{
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void UTargetingComponent::UpdateCamera()
{
	if (IsValid(Target))
	{
		if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(PlayerCharacter->GetController()))
		{
			const FRotator PlayerControllerRotator = PlayerController->GetControlRotation();
			const FVector CameraLocation = PlayerCharacter->GetSpringArmLocation();
			const FVector TargetLocation = Target->GetActorLocation();

			const FRotator LookAtTarget = UKismetMathLibrary::FindLookAtRotation(CameraLocation, TargetLocation);
			const FRotator RInterpToRotator = FMath::RInterpTo(PlayerControllerRotator, LookAtTarget,
				GetWorld()->GetDeltaSeconds(), InterpSpeed);

			PlayerController->SetControlRotation(RInterpToRotator);
		}
	}
}