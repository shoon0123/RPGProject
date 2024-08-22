// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/EnemyCharacterPDA.h"
#include "EnemyCharacterBossPDA.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UEnemyCharacterBossPDA : public UEnemyCharacterPDA
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float MeleeAttackDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float RangedAttackDistance;
};
