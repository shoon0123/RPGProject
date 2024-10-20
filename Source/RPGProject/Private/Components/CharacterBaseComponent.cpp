// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterBaseComponent.h"
#include "Character/CharacterBase.h"

UCharacterBaseComponent::UCharacterBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UCharacterBaseComponent::OnRegister()
{
	Super::OnRegister();

	InitializeOwner();
}

void UCharacterBaseComponent::InitializeOwner()
{
	OwnerCharacter = Cast<ACharacterBase>(GetOwner());
	CharacterInfo = OwnerCharacter->GetCharacterInfo();
}

