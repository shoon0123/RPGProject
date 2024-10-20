// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuOverlay.generated.h"

/**
 * 
 */
class UButton;

UCLASS()
class RPGPROJECT_API UMainMenuOverlay : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;

	UPROPERTY(EditAnywhere)
	FName StartLevelName;

	UFUNCTION(BlueprintCallable)
	void StartButtonCallback();
	
	UFUNCTION(BlueprintCallable)
	void QuitButtonCallback();

};
