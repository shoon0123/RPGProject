// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterAnimInstance.h"
#include "Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/WarriorPlayerController.h"

void UCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	PlayerController = Cast<APlayerController>(Character->GetController());
}


void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacterBase>(TryGetPawnOwner());
	if (IsValid(Character))
	{
		CharacterMovement = Character->GetCharacterMovement();
		WeaponSystem = Character->GetWeaponSystem();
	}
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (IsValid(CharacterMovement))
	{
		MovementAngle = UKismetMathLibrary::InverseTransformDirection(Character->GetActorTransform(), CharacterMovement->Velocity).Rotation().Yaw;
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);
		IsFalling = CharacterMovement->IsFalling();
	}
}

