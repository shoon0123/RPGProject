// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UCombatOverlay;

UCLASS()
class RPGPROJECT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	TObjectPtr<UCombatOverlay> GetCombatOverlay() const;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Slash)
	TSubclassOf<UCombatOverlay> CombatOverlayClass;
	
	UPROPERTY()
	TObjectPtr<UCombatOverlay> CombatOverlay;
};
