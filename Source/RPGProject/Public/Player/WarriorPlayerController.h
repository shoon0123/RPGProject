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

	void SetLockOnState(bool State);

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	//Input Actions
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

	bool bIsLockOn = false;

	void Attack();

	void Block();

	void BlockCancel();

	void Dodge();

	void EnableRun();

	void Jump();

	void LockOn();

	void Look(const FInputActionValue& InputActionValue);

	void Move(const FInputActionValue& InputActionValue);
};
