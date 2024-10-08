// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "AttributeWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UAttributeWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	void SetHealthPercent(float Percent);

	void SetPosturePercent(float Percent);

private:
	UPROPERTY()
	TObjectPtr<class UAttributeWidget> AttributeWidget;
};
