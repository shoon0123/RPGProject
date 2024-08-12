// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/EnemyCharacter.h"
#include "EnemyCharacterNormal.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AEnemyCharacterNormal : public AEnemyCharacter
{
	GENERATED_BODY()
	
public:
	AEnemyCharacterNormal();

	virtual void Attack() override;

	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter) override;

	virtual void GetPostureDamage(const float PostureDamage) override;

protected:
	virtual void BeginPlay() override;

	virtual void UpdateHealthBar() override;

	virtual void UpdatePostureBar() override;

	virtual void Die() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;

};
