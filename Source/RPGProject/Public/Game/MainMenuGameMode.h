// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
class UMainMenuOverlay;

UCLASS()
class RPGPROJECT_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;
	
private:
};