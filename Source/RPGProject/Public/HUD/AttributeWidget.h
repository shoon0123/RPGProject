// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributeWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UAttributeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PostureBar;
};
