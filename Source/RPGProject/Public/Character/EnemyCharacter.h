// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/HitInterface.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class RPGPROJECT_API AEnemyCharacter : public ACharacterBase, public IHitInterface
{
	GENERATED_BODY()
	
};
