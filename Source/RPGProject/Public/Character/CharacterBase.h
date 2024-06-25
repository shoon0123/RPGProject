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

UCLASS(Abstract)
class RPGPROJECT_API ACharacterBase : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual void Attack() PURE_VIRTUAL(ACharacterBase::Attack, );

	virtual void Destroyed() override;

	virtual UAnimMontage* GetAttackMontage() const;

	virtual EActionState GetActionState() const;

	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter) override;

	virtual void SetActionState(EActionState OtherActionState);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


protected:
	virtual void BeginPlay() override;

	virtual void Die();
	
	virtual void DirectionalHitReact(const AActor* Hitter);

	virtual void DestroyWeapon() PURE_VIRTUAL(ACharacterBase::DestroyWeapon, );

	virtual void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

	virtual void SpawnHitParticles();

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> AttackMontage;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> Attributes;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;

	UPROPERTY(VisibleAnywhere, Category = "State")
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	TObjectPtr<UParticleSystem> HitParticles;

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd() PURE_VIRTUAL(ACharacterBase::AttackEnd, );

	UFUNCTION(BlueprintCallable)
	virtual void HitReactEnd();

	virtual void SetupCollision();
};