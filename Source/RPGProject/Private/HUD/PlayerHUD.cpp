// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD.h"
#include "HUD/PlayerOverlay.h"

TObjectPtr<UPlayerOverlay> APlayerHUD::GetPlayerOverlay() const
{
	return PlayerOverlay;
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		TObjectPtr<APlayerController> Controller = World->GetFirstPlayerController();
		if (Controller && PlayerOverlayClass)
		{
			PlayerOverlay = CreateWidget<UPlayerOverlay>(Controller, PlayerOverlayClass);
			PlayerOverlay->SetPlayerHealthPercent(.1f);
			PlayerOverlay->SetEnemyHealthPercent(.1f);
			PlayerOverlay->AddToViewport();
		}
	}
}
