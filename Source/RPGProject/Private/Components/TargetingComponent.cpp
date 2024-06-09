// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TargetingComponent.h"

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

	for (TObjectPtr<AActor> Target : TargetableActors)
	{
		DrawDebugSphere(GetWorld(), Target->GetActorLocation(), 30.f, 20, FColor::Red, false);
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

bool UTargetingComponent::IsTargetable(TObjectPtr<AActor> Target)
{
	return Target->ActorHasTag(FName("Enemy"));
}

void UTargetingComponent::SetCollision()
{
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}
