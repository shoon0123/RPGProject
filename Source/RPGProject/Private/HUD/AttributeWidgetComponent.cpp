// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/AttributeWidgetComponent.h"
#include "HUD/AttributeWidget.h"
#include "Components/ProgressBar.h"

void UAttributeWidgetComponent::SetHealthPercent(float Percent)
{
	if (AttributeWidget == nullptr)
	{
		AttributeWidget = Cast<UAttributeWidget>(GetUserWidgetObject());
	}

	if (AttributeWidget && AttributeWidget->HealthBar)
	{
		AttributeWidget->HealthBar->SetPercent(Percent);
	}
}

void UAttributeWidgetComponent::SetPosturePercent(float Percent)
{
	if (AttributeWidget == nullptr)
	{
		AttributeWidget = Cast<UAttributeWidget>(GetUserWidgetObject());
	}

	if (AttributeWidget && AttributeWidget->PostureBar)
	{
		AttributeWidget->PostureBar->SetPercent(Percent);
	}
}
