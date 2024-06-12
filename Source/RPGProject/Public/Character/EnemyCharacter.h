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

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetActionState(EActionState OtherActionState) override;

	virtual void Attack() override;

	void SetCombatTarget(AActor* Target);

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AEnemyAIController> EnemyAIController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UBlueprint> WeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<AWeapon> Weapon;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> CombatTarget;

	virtual void BeginPlay() override;

	virtual void DestroyWeapon() override;

private:
	void SpawnWeapon();

	virtual void AttackEnd() override;
};
