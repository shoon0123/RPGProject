// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class AWeapon;
class UCameraComponent;
class UCombatOverlay;
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

	void DisableRun();

	void Dodge();

	void EnableRun();

	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter) override;

	TObjectPtr<UCombatOverlay> GetCombatOverlay() const;

	TObjectPtr<UTargetingComponent> GetTargetingComponent() const;

	FVector GetSpringArmLocation() const;

protected:
	virtual void BeginPlay() override;

	virtual void AttackEnd() override;

	virtual void Die() override;

	virtual void UpdateHealthBar() override;

	virtual void UpdatePostureBar() override;

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

	UPROPERTY(EditAnywhere, Category = "Targeting")
	TObjectPtr<UTargetingComponent> TargetingComponent;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float DodgingSpeed = 4000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkingSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunningSpeed = 800.f;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> BlockMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	TObjectPtr<USoundBase> BlockSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	TObjectPtr<USoundBase> ParryingSound;

	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	TObjectPtr<UParticleSystem> BlockParticles;

	bool bDoNextAttack = false;

	bool bParryingLeft = true;

	UFUNCTION(BlueprintCallable)
	void DodgeEnd();

	UFUNCTION(BlueprintCallable)
	void DisableParrying();

	UFUNCTION(BlueprintCallable)
	void EnableParrying();

	void ExecuteBlock(const FVector& ImpactPoint);

	void ExecuteParrying(const FVector& ImpactPoint);

	void PlayBlockSound(const FVector& ImpactPoint);

	void PlayParryingSound(const FVector& ImpactPoint);

	void SetupSpringArm();

	void SetupCamera();

	void SetupHUD();

	void SetupTargetingComponent();

	void SpawnBlockParticles(const FVector& ImpactPoint);

	void SpawnParryingParticles(const FVector& ImpactPoint);
};
