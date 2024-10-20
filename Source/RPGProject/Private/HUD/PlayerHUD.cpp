// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD.h"
#include "HUD/CombatOverlay.h"
#include "HUD/MainMenuOverlay.h"
#include "HUD/PauseOverlay.h"
#include "HUD/GameOverOverlay.h"

TObjectPtr<UCombatOverlay> APlayerHUD::GetCombatOverlay() const
{
	return CombatOverlay;
}

void APlayerHUD::SetCombatOverlay()
{
	TObjectPtr<APlayerController> Controller = GetWorld()->GetFirstPlayerController();
	if (IsValid(Controller))
	{
		if (CombatOverlay == nullptr && CombatOverlayClass)
		{
			CombatOverlay = CreateWidget<UCombatOverlay>(Controller, CombatOverlayClass);
			CombatOverlay->SetPlayerHealthPercent(1.f);
			CombatOverlay->SetEnemyHealthPercent(1.f);
		}
		CombatOverlay->AddToViewport();
	}
}

void APlayerHUD::SetMainMenuOverlay()
{
	TObjectPtr<APlayerController> Controller = GetWorld()->GetFirstPlayerController();
	if (IsValid(Controller))
	{
		if (MainMenuOverlay == nullptr && MainMenuOverlayClass)
		{
			MainMenuOverlay = CreateWidget<UMainMenuOverlay>(Controller, MainMenuOverlayClass);
		}
		MainMenuOverlay->AddToViewport();
	}
}

void APlayerHUD::SetPauseOverlay(bool bFlag)
{
	TObjectPtr<APlayerController> Controller = GetWorld()->GetFirstPlayerController();
	if (IsValid(Controller))
	{
		if (bFlag)
		{
			if (PauseOverlay == nullptr && PauseOverlayClass)
			{
				PauseOverlay = CreateWidget<UPauseOverlay>(Controller, PauseOverlayClass);
			}
			PauseOverlay->AddToViewport(1);
		}
		else if(PauseOverlay)
		{
			PauseOverlay->RemoveFromViewport();
		}
	}
}

void APlayerHUD::SetGameOverOverlay(bool bIsWin)
{
	TObjectPtr<APlayerController> Controller = GetWorld()->GetFirstPlayerController();
	if (IsValid(Controller))
	{
		if (GameOverOverlay == nullptr && GameOverOverlayClass)
		{
			GameOverOverlay = CreateWidget<UGameOverOverlay>(Controller, GameOverOverlayClass);
		}
		if (bIsWin)
		{
			GameOverOverlay->SetGameOverTextWin();
		}
		GameOverOverlay->AddToViewport();
	}
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	
}
