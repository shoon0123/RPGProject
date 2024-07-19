// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatOverlay.generated.h"

class UProgressBar;
class UImage;

UCLASS()
class RPGPROJECT_API UCombatOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetEnemyHealthPercent(float Percent);

	void SetEnemyPosturePercent(float Percent);

	void SetEnemyWidgetVisibility(ESlateVisibility InVisibility);

	void SetPlayerHealthPercent(float Percent);

	void SetPlayerPosturePercent(float Percent);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> EnemyHealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> EnemyPostureBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> EnemyPostureImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PlayerHealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PlayerPostureBar;
};
