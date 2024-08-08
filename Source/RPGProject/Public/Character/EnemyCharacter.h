// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "EnemyCharacter.generated.h"

class UBehaviorTree;
class AEnemyAIController;
class AWeapon;

UCLASS()
class RPGPROJECT_API AEnemyCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();

	virtual void Attack() override;

	virtual float GetDetectionRange() const;

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetActionState(EActionState OtherActionState) override;

	virtual void SetCombatTarget(ACharacter* Target);

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AEnemyAIController> EnemyAIController;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacter> CombatTarget;

	UPROPERTY(EditAnywhere)
	float DetectionRange = 2000.f;

	virtual void BeginPlay() override;

	virtual void AttackEnd() override;

	virtual void Die() override;

private:
};
