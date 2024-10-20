// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WarriorGameModeBase.h"
#include "HUD/CombatOverlay.h"
#include "HUD/PlayerHUD.h"
#include "Player/WarriorPlayerController.h"


void AWarriorGameModeBase::PlayerDied()
{
	GetWorld()->GetTimerManager().SetTimer(GameOverTimerHandle, this, &AWarriorGameModeBase::Lose, CoolDownToGameOver, false);
}

void AWarriorGameModeBase::EnemyBossDied()
{
	GetWorld()->GetTimerManager().SetTimer(GameOverTimerHandle, this, &AWarriorGameModeBase::Win, CoolDownToGameOver, false);
}

void AWarriorGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (PlayerHUD)
	{
		PlayerHUD->SetCombatOverlay();
		PlayerHUD->GetCombatOverlay()->SetEnemyWidgetVisibility(ESlateVisibility::Hidden);
	}
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
}

void AWarriorGameModeBase::Lose()
{
	if (PlayerHUD)
	{
		PlayerHUD->SetGameOverOverlay(false);
		PauseController();
	}
}

void AWarriorGameModeBase::Win()
{
	if (PlayerHUD)
	{
		PlayerHUD->SetGameOverOverlay(true);
		PauseController();
	}
}

void AWarriorGameModeBase::PauseController()
{
	TObjectPtr<AWarriorPlayerController> PlayerController = Cast<AWarriorPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->SwitchInputMode(true);
		PlayerController->SetPause(true);
	}
}
