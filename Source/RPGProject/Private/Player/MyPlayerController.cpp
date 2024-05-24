// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Weapon/Weapon.h"

AMyPlayerController::AMyPlayerController()
{
	bReplicates = true;
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//ULocalPlayer의 라이프타임과 동일하며 레벨 이동에 따라 이동된다.
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	check(Context);
	Subsystem->AddMappingContext(Context, 0);

	ControlledCharacter = GetPawn<APlayerCharacter>();

	bShowMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Jump);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMyPlayerController::Attack);
}

void AMyPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	const float WalkSpeed = 1 ;

	if (ControlledCharacter)
	{
		ControlledCharacter->AddMovementInput(ForwardDirection, InputAxisVector.Y * WalkSpeed);
		ControlledCharacter->AddMovementInput(RightDirection, InputAxisVector.X * WalkSpeed);
	}
}

void AMyPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	AddPitchInput(-InputAxisVector.Y);
	AddYawInput(InputAxisVector.X);
}

void AMyPlayerController::Jump()
{
	if (ControlledCharacter)
	{
		ControlledCharacter->Jump();
	}
}

void AMyPlayerController::Attack()
{
	if (ControlledCharacter)
	{
		if (ControlledCharacter->GetActionState() == EActionState::EAS_Attacking)
		{
			bDoNextAttack = true;
		}
		else
		{
			PlayAttackMontage();
			ControlledCharacter->SetActionState(EActionState::EAS_Attacking);
		}
	}
}

void AMyPlayerController::PlayAttackMontage()
{
	if (ControlledCharacter)
	{
		TObjectPtr<UAnimInstance> AnimInstance = ControlledCharacter->GetMesh()->GetAnimInstance();
		TObjectPtr<UAnimMontage> AttackMontage = ControlledCharacter->GetAttackMontage();
		check(AnimInstance);
		check(AttackMontage);
		AnimInstance->Montage_Play(AttackMontage);
		FName SectionName = FName("Attack1");
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AMyPlayerController::AttackEnd()
{
	if (!bDoNextAttack && ControlledCharacter)
	{
		ControlledCharacter->SetActionState(EActionState::EAS_Unoccupied);
		TObjectPtr<UAnimInstance> AnimInstance = ControlledCharacter->GetMesh()->GetAnimInstance();
		TObjectPtr<UAnimMontage> AttackMontage = ControlledCharacter->GetAttackMontage();
		check(AnimInstance);
		check(AttackMontage);
		AnimInstance->Montage_Stop(0.5f, AttackMontage);
	}
	bDoNextAttack = false;
}

void AMyPlayerController::SetWeaponCollisionEnable(bool bIsLeftHandWeapon, ECollisionEnabled::Type CollisionEnabled)
{
	if (ControlledCharacter)
	{
		if (bIsLeftHandWeapon)
		{
			ControlledCharacter->GetLeftHandWeapon()->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
			ControlledCharacter->GetLeftHandWeapon()->EmptyIgnoreActors();
		}
		else
		{
			ControlledCharacter->GetRightHandWeapon()->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
			ControlledCharacter->GetRightHandWeapon()->EmptyIgnoreActors();
		}
	}
}