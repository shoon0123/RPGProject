// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class AWeapon;
class USpringArmComponent;
class UCameraComponent;
class UTargetingComponent;

UCLASS()
class RPGPROJECT_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();

	virtual void Attack() override;

	TObjectPtr<UTargetingComponent> GetTargetingComponent();

	FVector GetCameraLocation() const;

protected:
	virtual void BeginPlay() override;

	virtual void DestroyWeapon() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<AWeapon> LeftHandWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<AWeapon> RightHandWeapon;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<UBlueprint> LeftHandWeaponType;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<UBlueprint> RightHandWeaponType;

	UPROPERTY(EditAnywhere, Category = "Targeting")
	TObjectPtr<UTargetingComponent> TargetingComponent;
	
	void SetSpringArm();

	void SetCamera();

	void SetTargetingComponent();
	
	void SpawnWeapons();

	bool bDoNextAttack = false;

	virtual void AttackEnd() override;
};
