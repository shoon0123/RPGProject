// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class AWeapon;
class UCameraComponent;
class UCombatOverlay;
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

	void Block();

	void BlockCancel();

	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter) override;

	TObjectPtr<UCombatOverlay> GetCombatOverlay() const;

	UFUNCTION(BlueprintCallable)
	UMovementAbilityComponent* GetMovementAbility() const;

	TObjectPtr<UTargetingComponent> GetTargetingComponent() const;

	FVector GetSpringArmLocation() const;

protected:
	virtual void BeginPlay() override;

	virtual void AttackEnd() override;

	virtual void Die() override;

	virtual void SetupData() override;

	virtual void UpdateHealthBar() override;

	virtual void UpdatePostureBar() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<AWeapon> LeftHandWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<AWeapon> RightHandWeapon;

	UPROPERTY(EditAnywhere, Category = "Dodge")
	TObjectPtr<UBlendSpace> DodgeBlendSpace;

private:
	UPROPERTY(EditAnywhere, Category = "Attribute")
	float ParryingPostureDamage;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, Category = "MovementAbility")
	TObjectPtr<UMovementAbilityComponent> MovementAbility;

	UPROPERTY(VisibleAnywhere, Category = "Targeting")
	TObjectPtr<UTargetingComponent> TargetingComponent;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> BlockMontage;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	TObjectPtr<USoundBase> BlockSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	TObjectPtr<USoundBase> ParryingSound;

	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	TObjectPtr<UParticleSystem> BlockParticles;

	bool bDoNextAttack = false;

	UFUNCTION(BlueprintCallable)
	void DisableParrying();

	UFUNCTION(BlueprintCallable)
	void EnableParrying();

	void ExecuteBlock(const FVector& ImpactPoint);

	void ExecuteGetPostureDamage(AActor* DamagedActor);

	void ExecuteParrying(const FVector& ImpactPoint, AActor* Hitter);

	void PlayBlockSound(const FVector& ImpactPoint);

	void PlayParryingSound(const FVector& ImpactPoint);

	void SetupSpringArm();

	void SetupCamera();

	void SetupHUD();

	void SetupMovementAbility();

	void SetupTargetingComponent();

	void SpawnBlockParticles(const FVector& ImpactPoint);

	void SpawnParryingParticles(const FVector& ImpactPoint);
};
