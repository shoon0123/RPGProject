// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverOverlay.generated.h"

/**
 * 
 */
class UButton;
class UTextBlock;

UCLASS()
class RPGPROJECT_API UGameOverOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetGameOverTextWin();

	void SetGameOverTextLose();

protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GameOverText;

	UPROPERTY(EditAnywhere)
	FName StartLevelName;

	UPROPERTY(EditAnywhere)
	FText WinText;

	UPROPERTY(EditAnywhere)
	FText LoseText;

	UFUNCTION(BlueprintCallable)
	void RestartButtonCallback();

	UFUNCTION(BlueprintCallable)
	void QuitButtonCallback();
};
