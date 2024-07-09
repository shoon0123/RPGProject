// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

class UProgressBar;

UCLASS()
class RPGPROJECT_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetPlayerHealthPercent(float Percent);

	void SetEnemyHealthPercent(float Percent);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PlayerHealthBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> EnemyHealthBar;
};
