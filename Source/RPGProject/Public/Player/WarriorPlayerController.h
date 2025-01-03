// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WarriorPlayerController.generated.h" 

class APlayerCharacter;
class UInputMappingContext;
class UInputAction;
class UCombatOverlay;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API AWarriorPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWarriorPlayerController();

	void Resume();

	void SetMouseSensitivity(float Value);

	void SwitchInputMode(bool bIsUIMode);

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* PawnToPossess) override;

	virtual void SetupInputComponent() override;

private:
	UPROPERTY(VisibleInstanceOnly, Category = "Player")
	TObjectPtr<APlayerCharacter> PlayerCharacter;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> Context;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> BlockAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> DodgeAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LockonAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> PauseAction;

	FInputModeGameOnly GameInputMode;

	FInputModeUIOnly   UIInputMode;

	float MouseSensitivity = 1.f;

	void Attack();

	void ExecuteBlock();

	void CancelBlock();

	void Dodge();

	void DisableRun();

	void EnableRun();

	void Jump();

	void LockOn();

	void Look(const FInputActionValue& InputActionValue);

	void Move(const FInputActionValue& InputActionValue);

	void Pause();
};
