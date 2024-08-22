// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/EnemyCharacter.h"
#include "EnemyCharacterBoss.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AEnemyCharacterBoss : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	virtual void Attack() override;

	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter) override;

	virtual void SetCombatTarget(ACharacter* Target) override;

protected:
	virtual void UpdateHealthBar() override;

	virtual void UpdatePostureBar() override;

	virtual void Die() override;

	virtual void SetupData() override;

private:
	UPROPERTY(EditAnywhere)
	float MeleeAttackDistance;

	UPROPERTY(EditAnywhere)
	float RangedAttackDistance;

	void MeleeAttack();
	
	void MidRangedAttack();
	
	void RangedAttack();
};
