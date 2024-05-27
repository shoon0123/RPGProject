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

	virtual EActionState GetActionState() const;
	virtual void SetActionState(EActionState OtherActionState);
	virtual UAnimMontage* GetAttackMontage() const;
	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;

	virtual void Attack() PURE_VIRTUAL(ACharacterBase::Attack, );

protected:
	virtual void BeginPlay() override;

	virtual void Die();
	virtual void DirectionalHitReact(const FVector& HitterLocation);
	virtual void PlayDeathMontage(const FName& SectionName);
	virtual void PlayHitReactMontage(const FName& SectionName);

	virtual void DestroyWeapon() PURE_VIRTUAL(ACharacterBase::DestroyWeapon, );

	virtual void PlayAttackMontage();

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

	// Animation Montages
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> HitReactMontage;
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	TObjectPtr<UParticleSystem> HitParticles;

	UFUNCTION(BlueprintCallable)
	virtual void HitReactEnd();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd() PURE_VIRTUAL(ACharacterBase::AttackEnd, );

};
