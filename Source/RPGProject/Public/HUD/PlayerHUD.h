// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UPlayerOverlay;

UCLASS()
class RPGPROJECT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	TObjectPtr<UPlayerOverlay> GetPlayerOverlay() const;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Slash)
	TSubclassOf<UPlayerOverlay> PlayerOverlayClass;
	
	UPROPERTY()
	TObjectPtr<UPlayerOverlay> PlayerOverlay;
};
