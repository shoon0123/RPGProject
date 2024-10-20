// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WarriorGameModeBase.generated.h"

/**
 * 
 */
class APlayerHUD;
UCLASS()
class RPGPROJECT_API AWarriorGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void PlayerDied();

	void EnemyBossDied();

protected:
	virtual void BeginPlay() override;

private:
	TObjectPtr<APlayerHUD> PlayerHUD;

	FTimerHandle GameOverTimerHandle;

	UPROPERTY(EditAnywhere)
	float CoolDownToGameOver = 3.f;

	void Lose();

	void Win();

	void PauseController();
};
