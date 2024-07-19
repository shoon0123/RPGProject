// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacterNormal.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"

AEnemyCharacterNormal::AEnemyCharacterNormal()
{
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
}

void AEnemyCharacterNormal::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit(ImpactPoint, Hitter);

	if (IsAlive())
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemyCharacterNormal::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemyCharacterNormal::UpdateHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

void AEnemyCharacterNormal::Die()
{
	Super::Die();
	HealthBarWidget->SetVisibility(false);
}
