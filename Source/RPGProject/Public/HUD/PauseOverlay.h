// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseOverlay.generated.h"

/**
 * 
 */
class UButton;
class USlider;

UCLASS()
class RPGPROJECT_API UPauseOverlay : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ResumeButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MouseSensSlider;

	UPROPERTY(EditAnywhere)
	FName StartLevelName;

	UFUNCTION(BlueprintCallable)
	void ResumeButtonCallback();

	UFUNCTION(BlueprintCallable)
	void RestartButtonCallback();

	UFUNCTION(BlueprintCallable)
	void QuitButtonCallback();

	UFUNCTION(BlueprintCallable)
	void SliderValueChanged(float Value);
};
