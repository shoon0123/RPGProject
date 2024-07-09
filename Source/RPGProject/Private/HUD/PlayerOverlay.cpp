// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlay.h"
#include"Components/ProgressBar.h"

void UPlayerOverlay::SetPlayerHealthPercent(float Percent)
{
	if (PlayerHealthBar)
	{
		PlayerHealthBar->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetEnemyHealthPercent(float Percent)
{
	if (EnemyHealthBar)
	{
		EnemyHealthBar->SetPercent(Percent);
	}
}
