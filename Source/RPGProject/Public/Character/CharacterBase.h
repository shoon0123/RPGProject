// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "Interaction/HitInterface.h"
#include "Interaction/PostureInterface.h"
#include "CharacterBase.generated.h"

class AWeapon;
class UAttributeComponent;
class UCharacterBasePDA;
class UHealthBarComponent;
class UWeaponSystemComponent;

UCLASS(Abstract)
class RPGPROJECT_API ACharacterBase : public ACharacter, public IHitInterface, public IPostureInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual void OnConstruction(const FTransform &Transform) override;

	virtual void Attack() PURE_VIRTUAL(ACharacterBase::Attack, );

	virtual void Destroyed() override;

	virtual EActionState GetActionState() const;

	virtual double GetAngle2DFromForwardVector(AActor* Actor) const;

	virtual double GetAngle2DFromForwardVector(const FVector& Vector) const;

	virtual TObjectPtr<UCharacterBasePDA> GetCharacterInfo() const;

	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter) override;

	virtual void GetPostureDamage(const float PostureDamage) override;

	virtual TObjectPtr<UWeaponSystemComponent> GetWeaponSystem() const;

	virtual bool IsAlive();

	virtual void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

	virtual void SetActionState(EActionState OtherActionState);

	virtual void Stunned();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void UpdateHealthBar() PURE_VIRTUAL(ACharacterBase::UpdateHealthBar, );

	virtual void UpdatePostureBar() PURE_VIRTUAL(ACharacterBase::UpdatePostureBar, );

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	virtual void AttackCoolDownEnd();

	virtual void Die();
	
	virtual void DirectionalHitReact(AActor* Hitter);

	UFUNCTION(BlueprintCallable)
	virtual void HitReactEnd();

	virtual void PlayHitSound(const FVector& ImpactPoint);

	virtual void SetupCollision();

	virtual void SetupData();

	virtual void SpawnHitParticles(const FVector& ImpactPoint);

	UFUNCTION(BlueprintCallable)
	virtual void StunnedEnd();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> Attributes;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWeaponSystemComponent> WeaponSystem;

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<UCharacterBasePDA> CharacterInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TArray<FName> AttackMontageSections;

	FTimerHandle AttackEndTimerHandle;

	float CoolDownToAttack;

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
};