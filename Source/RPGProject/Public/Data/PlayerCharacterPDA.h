// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterBasePDA.h"
#include "PlayerCharacterPDA.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UPlayerCharacterPDA : public UCharacterBasePDA
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Attribute")
	float ParryingPostureDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float DodgingSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float WalkingSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float RunningSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> BlockMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	TObjectPtr<USoundBase> BlockSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	TObjectPtr<USoundBase> ParryingSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VisualEffects")
	TObjectPtr<UParticleSystem> BlockParticles;
};
