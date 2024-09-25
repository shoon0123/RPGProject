// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StateSystemComponent.h"

EActionState UStateSystemComponent::GetActionState() const
{
	return ActionState;
}

void UStateSystemComponent::SetActionState(EActionState NewActionState)
{
	ActionState = NewActionState;
}
