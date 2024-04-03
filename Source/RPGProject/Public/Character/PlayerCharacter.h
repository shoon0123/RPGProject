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
	
	UFUNCTION()
	UAnimMontage* GetAttackMontage() const;
	UFUNCTION()
	EActionState GetActionState() const;
	UFUNCTION()
	void SetActionState(EActionState OtherActionState);
	UFUNCTION()
	AWeapon* GetLeftHandWeapon() const;
	UFUNCTION()
	AWeapon* GetRightHandWeapon() const;



protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "State")
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<UBlueprint> LeftHandWeaponType;
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<UBlueprint> RightHandWeaponType;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<AWeapon> LeftHandWeapon;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<AWeapon> RightHandWeapon;

	//Animation Montages
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> AttackMontage;


	void SetSpringArm();
	void SetCamera();
	void SpawnWeapons();
};
