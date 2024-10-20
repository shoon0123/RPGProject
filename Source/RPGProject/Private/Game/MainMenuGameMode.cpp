// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MainMenuGameMode.h"
#include "HUD/PlayerHUD.h"

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<APlayerHUD> PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (PlayerHUD)
	{
		PlayerHUD->SetMainMenuOverlay();
	}
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}
