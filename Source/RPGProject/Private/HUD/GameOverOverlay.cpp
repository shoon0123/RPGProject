// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/GameOverOverlay.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Player/WarriorPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UGameOverOverlay::SetGameOverTextWin()
{
	GameOverText->SetText(WinText);
}

void UGameOverOverlay::SetGameOverTextLose()
{
	GameOverText->SetText(LoseText);
}

void UGameOverOverlay::NativeOnInitialized()
{
	RestartButton->OnClicked.AddDynamic(this, &UGameOverOverlay::RestartButtonCallback);
	QuitButton->OnClicked.AddDynamic(this, &UGameOverOverlay::QuitButtonCallback);
	SetGameOverTextLose();
}

void UGameOverOverlay::RestartButtonCallback()
{
	TObjectPtr<AWarriorPlayerController> PlayerController = Cast<AWarriorPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SwitchInputMode(false);
		PlayerController->SetPause(false);
	}
	UGameplayStatics::OpenLevel(this, StartLevelName);
}

void UGameOverOverlay::QuitButtonCallback()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, true);
}
