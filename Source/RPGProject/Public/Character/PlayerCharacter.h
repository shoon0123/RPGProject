// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class AWeapon;
class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

private:
	void SetSpringArm();
	void SetCamera();

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm; 
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Weapon")
	TObjectPtr<UBlueprint> LeftHandWeapon;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Weapon")
	TObjectPtr<UBlueprint> RightHandWeapon;
};
