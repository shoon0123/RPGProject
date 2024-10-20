// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UCombatOverlay;
class UMainMenuOverlay;
class UPauseOverlay;
class UGameOverOverlay;

UCLASS()
class RPGPROJECT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	TObjectPtr<UCombatOverlay> GetCombatOverlay() const;

	void SetCombatOverlay();
	void SetMainMenuOverlay();
	void SetPauseOverlay(bool bFlag);
	void SetGameOverOverlay(bool bIsWin);
	

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = CombatOverlay)
	TSubclassOf<UCombatOverlay> CombatOverlayClass;
	UPROPERTY()
	TObjectPtr<UCombatOverlay> CombatOverlay;

	UPROPERTY(EditDefaultsOnly, Category = MainMenuOverlay)
	TSubclassOf<UMainMenuOverlay> MainMenuOverlayClass;
	UPROPERTY()
	TObjectPtr<UMainMenuOverlay> MainMenuOverlay;

	UPROPERTY(EditDefaultsOnly, Category = PauseOverlay)
	TSubclassOf<UPauseOverlay> PauseOverlayClass;
	UPROPERTY()
	TObjectPtr<UPauseOverlay> PauseOverlay;

	UPROPERTY(EditDefaultsOnly, Category = GameOverOverlay)
	TSubclassOf<UGameOverOverlay> GameOverOverlayClass;
	UPROPERTY()
	TObjectPtr<UGameOverOverlay> GameOverOverlay;
};
