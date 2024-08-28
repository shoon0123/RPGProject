// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacterNormal.h"
#include "Components/AttributeComponent.h"
#include "Data/EnemyCharacterNormalPDA.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"

AEnemyCharacterNormal::AEnemyCharacterNormal()
{
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
}

void AEnemyCharacterNormal::Attack()
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}
	if (GetActionState() != EActionState::EAS_Unoccupied)
	{
		return;
	}

	if (!AttackMontageSections.IsEmpty())
	{
		SetActionState(EActionState::EAS_Attacking);
		const int32 Selection = FMath::RandRange(0, AttackMontageSections.Num() - 1);
		PlayMontageSection(AttackMontage, AttackMontageSections[Selection]);
	}
}

void AEnemyCharacterNormal::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit(ImpactPoint, Hitter);

	if (IsAlive())
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemyCharacterNormal::GetPostureDamage(const float PostureDamage)
{
	Super::GetPostureDamage(PostureDamage);

	if (IsAlive())
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemyCharacterNormal::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(HealthBarWidget))
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemyCharacterNormal::UpdateHealthBar()
{
	if (IsValid(HealthBarWidget))
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

void AEnemyCharacterNormal::UpdatePostureBar()
{
	if (IsValid(HealthBarWidget))
	{
		HealthBarWidget->SetPosturePercent(Attributes->GetPosturePercent());
	}
}

void AEnemyCharacterNormal::Die()
{
	Super::Die();

	if (IsValid(HealthBarWidget))
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemyCharacterNormal::SetupData()
{
	Super::SetupData();
}
