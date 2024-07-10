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

	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetActionState(EActionState OtherActionState) override;

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

	virtual void AttackEnd() override;

	virtual void DestroyWeapon() override;

	virtual void UpdateHealthBar() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;

	void SpawnWeapon();
};
