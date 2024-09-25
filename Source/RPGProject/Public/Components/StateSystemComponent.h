// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterBaseComponent.h"
#include "Character/CharacterTypes.h"
#include "StateSystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UStateSystemComponent : public UCharacterBaseComponent
{
	GENERATED_BODY()

public:
	//UStateSystemComponent();

	EActionState GetActionState() const;

	void SetActionState(EActionState NewActionState);
	

private:
	UPROPERTY(VisibleAnywhere, Category = "State")
	EActionState ActionState = EActionState::EAS_Unoccupied;


};
