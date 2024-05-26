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

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY()
	TObjectPtr<AEnemyAIController> EnemyAIController;

	virtual void BeginPlay() override;
	virtual void DestroyWeapon() override;

private:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<UBlueprint> WeaponType;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<AWeapon> Weapon;

	void SpawnWeapon();

	virtual void AttackEnd() override;
};
