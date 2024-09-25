// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class AWeapon;
class UCameraComponent;
class UCombatOverlay;
class UGuardAbilityComponent;
class UMovementAbilityComponent;
class USpringArmComponent;
class UTargetingComponent;

UCLASS()
class RPGPROJECT_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();

	virtual void Attack() override;

	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter) override;

	TObjectPtr<UCombatOverlay> GetCombatOverlay() const;

	TObjectPtr<UGuardAbilityComponent> GetGuardAbility() const;

	TObjectPtr<UMovementAbilityComponent> GetMovementAbility() const;

	TObjectPtr<UTargetingComponent> GetTargetingComponent() const;

	FVector GetSpringArmLocation() const;

protected:
	virtual void BeginPlay() override;

	virtual void AttackEnd() override;

	virtual void AttackCoolDownEnd() override;

	virtual void Die() override;

	virtual void SetupData() override;

	virtual void UpdateHealthBar() override;

	virtual void UpdatePostureBar() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GuardAbility")
	TObjectPtr<UGuardAbilityComponent> GuardAbility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovementAbility")
	TObjectPtr<UMovementAbilityComponent> MovementAbility;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, Category = "Targeting")
	TObjectPtr<UTargetingComponent> TargetingComponent;

	void ComboAttack();

	bool bDoNextAttack = false;

	bool bIsAttackCoolDown = false;

	uint8 ComboCount = 0;

	void SetupSpringArm();

	void SetupCamera();

	void SetupHUD();
};
