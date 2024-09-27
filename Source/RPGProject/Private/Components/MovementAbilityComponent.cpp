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
    if (IsValid(OwnerCharacter))
    {
        if (OwnerCharacter->GetActionState() == EActionState::EAS_Unoccupied && !OwnerCharacter->GetCharacterMovement()->IsFalling())
        {
            const FVector VelocityNormal2D = OwnerCharacter->GetCharacterMovement()->Velocity.GetSafeNormal2D();
            const float Angle = OwnerCharacter->GetAngle2DFromForwardVector(OwnerCharacter->GetActorLocation()+ VelocityNormal2D);

            if (VelocityNormal2D.IsZero())
            {
                OwnerCharacter->SetActionState(EActionState::EAS_Dodge);
                OwnerCharacter->PlayMontageSection(DodgeMontage, FName("DodgeForward"));
            }
            else if ( (- 90.f < Angle && Angle < 90.f))
            {
                OwnerCharacter->SetActorRotation(VelocityNormal2D.Rotation());
                OwnerCharacter->SetActionState(EActionState::EAS_Dodge);
                OwnerCharacter->PlayMontageSection(DodgeMontage, FName("DodgeForward"));
            }
            else
            {
                OwnerCharacter->SetActorRotation((-VelocityNormal2D).Rotation());
                OwnerCharacter->SetActionState(EActionState::EAS_Dodge);
                OwnerCharacter->PlayMontageSection(DodgeMontage, FName("DodgeBackward"));
            }
        }
    }
   
}

void UMovementAbilityComponent::EnableRun()
{
    if (IsValid(OwnerCharacter))
    {
        OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
    }
}

void UMovementAbilityComponent::DisableRun()
{
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
    if (IsValid(OwnerCharacter))
    {
        OwnerCharacter->SetActionState(EActionState::EAS_Unoccupied);
    }
}

