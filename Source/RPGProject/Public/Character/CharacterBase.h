// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "Interaction/HitInterface.h"
#include "CharacterBase.generated.h"

class UAttributeComponent;
class UHealthBarComponent;

UCLASS(Abstract)
class RPGPROJECT_API ACharacterBase : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	UFUNCTION()
	EActionState GetActionState() const;

	UFUNCTION()
	void SetActionState(EActionState OtherActionState);

	UFUNCTION()
	UAnimMontage* GetAttackMontage() const;

	UFUNCTION()
	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter);

	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	virtual void DirectionalHitReact(const FVector& HitterLocation);
	virtual void PlayHitReactMontage(const FName& SectionName);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> Attributes;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;

	UPROPERTY(VisibleAnywhere, Category = "State")
	EActionState ActionState = EActionState::EAS_Unoccupied;

	// Animation Montages
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	TObjectPtr<UParticleSystem> HitParticles;
};
