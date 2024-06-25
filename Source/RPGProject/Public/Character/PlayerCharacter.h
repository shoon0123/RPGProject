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

	void Block();

	void BlockCancel();

	void Dodge();

	void EnableRun();

	void DisableRun();

	TObjectPtr<UTargetingComponent> GetTargetingComponent() const;

	FVector GetSpringArmLocation() const;

protected:
	virtual void BeginPlay() override;

	virtual void DestroyWeapon() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<AWeapon> LeftHandWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<AWeapon> RightHandWeapon;

	UPROPERTY(EditAnywhere, Category = "Dodge")
	TObjectPtr<UBlendSpace> DodgeBlendSpace;

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

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkingSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunningSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float DodgingSpeed = 4000.f;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> BlockMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> DodgeMontage;

	bool bDoNextAttack = false;

	virtual void AttackEnd() override;

	UFUNCTION(BlueprintCallable)
	void DodgeEnd();

	void SetupSpringArm();

	void SetupCamera();

	void SetupTargetingComponent();
	
	void SpawnWeapons();
};
