// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"


UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

void UAttributeComponent::ReceivePostureDamage(float PostureDamage)
{
	Posture = FMath::Clamp(Posture + PostureDamage, 0.f, MaxPosture);
}

void UAttributeComponent::RecoverHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.f, MaxHealth);
}

void UAttributeComponent::RecoverPosture(float Amount)
{
	Posture = FMath::Clamp(Posture - Amount, 0.f, MaxPosture);
}

void UAttributeComponent::SetPostureZero()
{
	Posture = 0;
}

void UAttributeComponent::SetHealth(float Amount)
{
	Health = Amount;
}

void UAttributeComponent::SetMaxHealth(float Amount)
{
	MaxHealth = Amount;
}

void UAttributeComponent::SetPosture(float Amount)
{
	Posture = Amount;
}

void UAttributeComponent::SetMaxPosture(float Amount)
{
	MaxPosture = Amount;
}

float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

float UAttributeComponent::GetPosturePercent()
{
	return Posture / MaxPosture;
}

bool UAttributeComponent::IsAlive()
{
	return Health > 0.f;
}

bool UAttributeComponent::IsPostureBroken()
{
	return Posture >= MaxPosture;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

}



