// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MovementAbilityComponent.h"
#include "Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UMovementAbilityComponent::UMovementAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UMovementAbilityComponent::Dodge()
{
    TObjectPtr<ACharacterBase> OwnerCharacter = Cast< ACharacterBase>(GetOwner());
    if (IsValid(OwnerCharacter))
    {
        if (OwnerCharacter->GetActionState() == EActionState::EAS_Unoccupied && !OwnerCharacter->GetCharacterMovement()->IsFalling())
        {
            OwnerCharacter->SetActionState(EActionState::EAS_Dodge);
            OwnerCharacter->PlayMontageSection(DodgeMontage, FName("Dodge"));

            if (OwnerCharacter->GetCharacterMovement()->Velocity.IsZero())
            {
                OwnerCharacter->LaunchCharacter(OwnerCharacter->GetActorForwardVector() * DodgingSpeed, true, true);
            }
            else
            {
                OwnerCharacter->LaunchCharacter(OwnerCharacter->GetCharacterMovement()->Velocity.GetSafeNormal2D() * DodgingSpeed, true, true);
            }
        }
    }
   
}

void UMovementAbilityComponent::EnableRun()
{
    TObjectPtr<ACharacterBase> OwnerCharacter = Cast<ACharacterBase>(GetOwner());
    if (IsValid(OwnerCharacter))
    {
        OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
    }
}

void UMovementAbilityComponent::DisableRun()
{
    TObjectPtr<ACharacterBase> OwnerCharacter = Cast<ACharacterBase>(GetOwner());
    if (IsValid(OwnerCharacter))
    {
        OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
    }
}


// Called when the game starts
void UMovementAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMovementAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMovementAbilityComponent::DodgeEnd()
{
    TObjectPtr<ACharacterBase> OwnerCharacter = Cast< ACharacterBase>(GetOwner());
    if (IsValid(OwnerCharacter))
    {
        OwnerCharacter->SetActionState(EActionState::EAS_Unoccupied);
    }
    DisableRun();
}

