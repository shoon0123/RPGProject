// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD.h"
#include "HUD/CombatOverlay.h"

TObjectPtr<UCombatOverlay> APlayerHUD::GetCombatOverlay() const
{
	return CombatOverlay;
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	TObjectPtr<UWorld> World = GetWorld();
	if (IsValid(World))
	{
		TObjectPtr<APlayerController> Controller = World->GetFirstPlayerController();
		if (IsValid(Controller) && IsValid(CombatOverlayClass))
		{
			CombatOverlay = CreateWidget<UCombatOverlay>(Controller, CombatOverlayClass);
			CombatOverlay->SetPlayerHealthPercent(1.f);
			CombatOverlay->SetEnemyHealthPercent(1.f);
			CombatOverlay->AddToViewport();
		}
	}
}
