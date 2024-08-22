// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterBasePDA.h"
#include "EnemyCharacterPDA.generated.h"

class UBehaviorTree;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API UEnemyCharacterPDA : public UCharacterBasePDA
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float DetectionRange;



};
