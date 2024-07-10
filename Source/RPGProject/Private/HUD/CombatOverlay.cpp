// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CombatOverlay.h"
#include "Components/ProgressBar.h"

void UCombatOverlay::SetPlayerHealthPercent(float Percent)
{
	if (PlayerHealthBar)
	{
		PlayerHealthBar->SetPercent(Percent);
	}
}

void UCombatOverlay::SetEnemyHealthPercent(float Percent)
{
	if (EnemyHealthBar)
	{
		EnemyHealthBar->SetPercent(Percent);
	}
}
