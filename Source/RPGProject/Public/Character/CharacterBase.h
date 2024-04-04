// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "Interaction/HitInterface.h"
#include "CharacterBase.generated.h"

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

	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter);

protected:
	virtual void BeginPlay() override;

	virtual void PlayHitReactMontage(const FName& SectionName);

	virtual void DirectionalHitReact(const FVector& HitterLocation);

private:
	UPROPERTY(VisibleAnywhere, Category = "State")
	EActionState ActionState = EActionState::EAS_Unoccupied;

	// Animation Montages
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
