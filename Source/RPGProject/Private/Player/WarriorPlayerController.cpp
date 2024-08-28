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
	TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>();
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->Attack();
	}
}

void AWarriorPlayerController::Block()
{
	TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>()
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->Block();
	}
}

void AWarriorPlayerController::BlockCancel()
{
	TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>();
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->BlockCancel();
	}
}

void AWarriorPlayerController::Dodge()
{
	TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>();
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->Dodge();
	}
}

void AWarriorPlayerController::EnableRun()
{
	TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>();
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->EnableRun();
	}
}

void AWarriorPlayerController::Jump()
{
	TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>();
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->Jump();
	}
}

void AWarriorPlayerController::LockOn()
{
	TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>();
	if (IsValid(PlayerCharacter))
	{
		TObjectPtr<UTargetingComponent> TargetingComponent = PlayerCharacter->GetTargetingComponent();
		if (IsValid(TargetingComponent))
		{
			if (TargetingComponent->IsLockOn())
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
	TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>();
	if (IsValid(PlayerCharacter))
	{
		TObjectPtr<UTargetingComponent> TargetingComponent = PlayerCharacter->GetTargetingComponent();
		if (IsValid(TargetingComponent)&& TargetingComponent->IsLockOn())
		{
			TargetingComponent->ChangeLockOnTarget(InputAxisVector);
			return;
		}
		AddPitchInput(-InputAxisVector.Y);
		AddYawInput(InputAxisVector.X);
	}
}

void AWarriorPlayerController::Move(const FInputActionValue& InputActionValue)
{
	TObjectPtr<APlayerCharacter> PlayerCharacter = GetPawn<APlayerCharacter>();
	if (IsValid(PlayerCharacter))
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
