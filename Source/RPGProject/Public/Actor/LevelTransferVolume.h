// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransferVolume.generated.h"

class UBoxComponent;

UCLASS()
class RPGPROJECT_API ALevelTransferVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelTransferVolume();

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere)
	FName TransferLevelName;

private:
	TObjectPtr<UBoxComponent> TransferVolume;

};
