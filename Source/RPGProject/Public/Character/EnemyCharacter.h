// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "EnemyCharacter.generated.h"

class UBehaviorTree;
class AEnemyAIController;
class AWeapon;

UCLASS(Abstract)
class RPGPROJECT_API AEnemyCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();

	virtual void Attack() override;

	virtual float GetDetectionRange() const;

	virtual TObjectPtr<ACharacterBase> GetCombatTarget() const;

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetActionState(EActionState OtherActionState) override;

	virtual void SetCombatTarget(ACharacterBase* NewTarget);

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AEnemyAIController> EnemyAIController;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacterBase> CombatTarget;

	UPROPERTY(EditAnywhere)
	float DetectionRange;

	virtual void BeginPlay() override;

	virtual void AttackEnd() override;

	virtual void Die() override;

	virtual void SetupData() override;

private:
};
