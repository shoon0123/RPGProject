// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "EnemyCharacter.generated.h"

class UBehaviorTree;
class AEnemyAIController;

UCLASS()
class RPGPROJECT_API AEnemyCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();

	virtual void PossessedBy(AController* NewController) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AEnemyAIController> EnemyAIController;

};
