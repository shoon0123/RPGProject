// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CombatOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UCombatOverlay::SetEnemyWidgetVisibility(ESlateVisibility InVisibility)
{
	if (IsValid(EnemyHealthBar) && IsValid(EnemyPostureBar) && IsValid(EnemyPostureImage))
	{
		EnemyHealthBar->SetVisibility(InVisibility);
		EnemyPostureBar->SetVisibility(InVisibility);
		EnemyPostureImage->SetVisibility(InVisibility);
	}
}

void UCombatOverlay::SetPlayerHealthPercent(float Percent)
{
	if (IsValid(PlayerHealthBar))
	{
		PlayerHealthBar->SetPercent(Percent);
	}
}

void UCombatOverlay::SetPlayerPosturePercent(float Percent)
{
	if (IsValid(PlayerPostureBar))
	{
		PlayerPostureBar->SetPercent(Percent);
	}
}

void UCombatOverlay::SetEnemyHealthPercent(float Percent)
{
	if (IsValid(EnemyHealthBar))
	{
		EnemyHealthBar->SetPercent(Percent);
	}
}

void UCombatOverlay::SetEnemyPosturePercent(float Percent)
{
	if (IsValid(EnemyPostureBar))	
	{
		EnemyPostureBar->SetPercent(Percent);
	}
}
