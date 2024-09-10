// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterBaseComponent.generated.h"

class ACharacterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API UCharacterBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterBaseComponent();

protected:
	virtual void OnRegister() override;

		
protected:
	UPROPERTY()
	TObjectPtr<ACharacterBase> OwnerCharacter;

	virtual void InitializeOwner();



};
