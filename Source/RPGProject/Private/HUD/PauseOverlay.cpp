// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PauseOverlay.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Player/WarriorPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UPauseOverlay::NativeOnInitialized()
{
	ResumeButton->OnClicked.AddDynamic(this, &UPauseOverlay::ResumeButtonCallback);
	RestartButton->OnClicked.AddDynamic(this, &UPauseOverlay::RestartButtonCallback);
	QuitButton->OnClicked.AddDynamic(this, &UPauseOverlay::QuitButtonCallback); 
	MouseSensSlider->OnValueChanged.AddDynamic(this, &UPauseOverlay::SliderValueChanged);
	MouseSensSlider->SetValue(1.f);
}

void UPauseOverlay::ResumeButtonCallback()
{
	TObjectPtr<AWarriorPlayerController> PlayerController = Cast<AWarriorPlayerController>(GetOwningPlayer());
	if (PlayerController)
	{
		PlayerController->Resume();
	}
}

void UPauseOverlay::RestartButtonCallback()
{
	TObjectPtr<AWarriorPlayerController> PlayerController = Cast<AWarriorPlayerController>(GetOwningPlayer());
	if (PlayerController)
	{
		PlayerController->Resume();
		UGameplayStatics::OpenLevel(this, StartLevelName);
	}
}

void UPauseOverlay::QuitButtonCallback()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, true);
}

void UPauseOverlay::SliderValueChanged(float Value)
{
	TObjectPtr<AWarriorPlayerController> PlayerController = Cast<AWarriorPlayerController>(GetOwningPlayer());
	if (PlayerController)
	{
		PlayerController->SetMouseSensitivity(Value);
	}
}
