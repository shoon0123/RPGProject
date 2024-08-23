// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "Interaction/HitInterface.h"
#include "CharacterBase.generated.h"

class UAttributeComponent;
class UHealthBarComponent;
class AWeapon;
class UCharacterBasePDA;

UCLASS(Abstract)
class RPGPROJECT_API ACharacterBase : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual void OnConstruction(const FTransform &Transform) override;

	virtual void Attack() PURE_VIRTUAL(ACharacterBase::Attack, );

	virtual void Destroyed() override;

	virtual UAnimMontage* GetAttackMontage() const;

	virtual EActionState GetActionState() const;

	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter) override;

	virtual void GetPostureDamage(const float PostureDamage) override;

	virtual void GetStunned();

	virtual bool IsAlive();

	virtual void SetActionState(EActionState OtherActionState);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd() PURE_VIRTUAL(ACharacterBase::AttackEnd, );

	virtual void Die();
	
	virtual void DirectionalHitReact(AActor* Hitter);

	virtual void DestroyWeapons();

	virtual double GetAngleXYFromForwardVector(AActor* Actor) const;

	virtual double GetAngleXYFromForwardVector(const FVector& Vector) const;

	UFUNCTION(BlueprintCallable)
	virtual void HitReactEnd();

	virtual void PlayHitSound(const FVector& ImpactPoint);

	virtual void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

	virtual void RecoverCondition();

	virtual void SetupCollision();

	virtual void SetupData();

	virtual void SetWeaponsCollisionDisable();

	virtual void SpawnHitParticles(const FVector& ImpactPoint);

	virtual void SpawnWeapons();

	UFUNCTION(BlueprintCallable)
	virtual void StunnedEnd();

	virtual void UpdateHealthBar() PURE_VIRTUAL(ACharacterBase::UpdateHealthBar, );

	virtual void UpdatePostureBar() PURE_VIRTUAL(ACharacterBase::UpdatePostureBar, );

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> Attributes;

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<UCharacterBasePDA> CharacterInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<TObjectPtr<AWeapon>> Weapons;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float RecoveryPerSec;

private:
	UPROPERTY(VisibleAnywhere, Category = "State")
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TArray<FName> DeathMontageSections;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TArray<FName> HitReactMontageSections;
	
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> StunnedMontage;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	TObjectPtr<UParticleSystem> HitParticles;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TArray<TObjectPtr<UBlueprint>> WeaponTypes;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TMap<TObjectPtr<UBlueprint>, FName> WeaponTypeSocketMap;
};