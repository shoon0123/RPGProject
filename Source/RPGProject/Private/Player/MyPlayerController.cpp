// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WarriorPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/PlayerCharacter.h"
#include "Character/EnemyCharacter.h"
#include "Components/TargetingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AWarriorPlayerController::AWarriorPlayerController()
{
	bReplicates = true;
}

void AWarriorPlayerController::SetLockOnState(bool State)
{
	bIsLockOn = State;
}

void AWarriorPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//ULocalPlayer의 라이프타임과 동일하며 레벨 이동에 따라 이동된다.
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	check(Context);
	Subsystem->AddMappingContext(Context, 0);

	bShowMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AWarriorPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AWarriorPlayerController::Attack);
	EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Triggered, this, &AWarriorPlayerController::Block);
	EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &AWarriorPlayerController::BlockCancel);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Completed, this, &AWarriorPlayerController::Dodge);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &AWarriorPlayerController::EnableRun);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AWarriorPlayerController::Jump);
	EnhancedInputComponent->BindAction(LockonAction, ETriggerEvent::Started, this, &AWarriorPlayerController::LockOn);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWarriorPlayerController::Look);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWarriorPlayerController::Move);
}

void AWarriorPlayerController::Attack()
{
	if (TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		PlayerCharacter->Attack();
	}
}

void AWarriorPlayerController::Block()
{
	if (TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		PlayerCharacter->Block();
	}
}

void AWarriorPlayerController::BlockCancel()
{
	if (TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		PlayerCharacter->BlockCancel();
	}
}

void AWarriorPlayerController::Dodge()
{
	if (TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		PlayerCharacter->Dodge();
	}
}

void AWarriorPlayerController::EnableRun()
{
	if (TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		PlayerCharacter->EnableRun();
	}
}

void AWarriorPlayerController::Jump()
{
	if (GetPawn<ACharacter>())
	{
		GetPawn<ACharacter>()->Jump();
	}
}

void AWarriorPlayerController::LockOn()
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

void AWarriorPlayerController::Look(const FInputActionValue& InputActionValue)
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

void AWarriorPlayerController::Move(const FInputActionValue& InputActionValue)
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
