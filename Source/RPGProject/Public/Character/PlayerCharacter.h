// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "CharacterTypes.h"
#include "PlayerCharacter.generated.h"

class AWeapon;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class RPGPROJECT_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();
	UAnimMontage* GetAttackMontage() const;
	EActionState GetActionState() const;
	void SetActionState(EActionState OtherActionState);


protected:
	virtual void BeginPlay() override;

private:
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm; 
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<UBlueprint> LeftHandWeapon;
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<UBlueprint> RightHandWeapon;

	//Animation Montages
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> AttackMontage;



	void SetSpringArm();
	void SetCamera();

};
