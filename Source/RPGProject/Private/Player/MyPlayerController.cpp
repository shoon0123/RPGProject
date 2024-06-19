// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/PlayerCharacter.h"
#include "Character/EnemyCharacter.h"
#include "Components/TargetingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyPlayerController::AMyPlayerController()
{
	bReplicates = true;
}

void AMyPlayerController::SetLockOnState(bool State)
{
	bIsLockOn = State;
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//ULocalPlayer의 라이프타임과 동일하며 레벨 이동에 따라 이동된다.
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	check(Context);
	Subsystem->AddMappingContext(Context, 0);

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
	EnhancedInputComponent->BindAction(LockonAction, ETriggerEvent::Started, this, &AMyPlayerController::LockOn);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &AMyPlayerController::EnableRun);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Completed, this, &AMyPlayerController::Dodge);
}

void AMyPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		if (PlayerCharacter->GetActionState() != EActionState::EAS_Unoccupied)
		{
			return;
		}
		const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
		PlayerCharacter->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		PlayerCharacter->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AMyPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	if (bIsLockOn)
	{
		if (TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>())
		{
			if (TObjectPtr<UTargetingComponent> TargetingComponent = PlayerCharacter->GetTargetingComponent())
			{
				TargetingComponent->ChangeLockOnTarget(InputAxisVector);
			}
		}
	}
	else
	{
		AddPitchInput(-InputAxisVector.Y);
		AddYawInput(InputAxisVector.X);
	}
}

void AMyPlayerController::Jump()
{
	if (GetPawn<ACharacter>())
	{
		GetPawn<ACharacter>()->Jump();
	}
}

void AMyPlayerController::Attack()
{
	if (GetPawn<APlayerCharacter>())
	{
		GetPawn<APlayerCharacter>()->Attack();
	}
}

void AMyPlayerController::LockOn()
{
	if (TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		if (TObjectPtr<UTargetingComponent> TargetingComponent = PlayerCharacter->GetTargetingComponent())
		{
			if (bIsLockOn)
			{
				TargetingComponent->CancelLockOn();
			}
			else
			{
				TargetingComponent->ExecuteLockOn();
			}
		}
	}
}

void AMyPlayerController::Dodge()
{
	if (TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		PlayerCharacter->Dodge();
	}
}

void AMyPlayerController::EnableRun()
{
	if (TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		PlayerCharacter->EnableRun();
	}
}
