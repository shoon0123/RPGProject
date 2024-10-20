// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainMenuOverlay.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuOverlay::NativeOnInitialized()
{
	StartButton->OnClicked.AddDynamic(this, &UMainMenuOverlay::StartButtonCallback);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenuOverlay::QuitButtonCallback);
}

void UMainMenuOverlay::StartButtonCallback()
{
	UGameplayStatics::OpenLevel(this, StartLevelName);
}

void UMainMenuOverlay::QuitButtonCallback()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, true);
}
