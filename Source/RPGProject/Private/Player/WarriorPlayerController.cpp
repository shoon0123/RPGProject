// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WarriorPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/PlayerCharacter.h"
#include "Character/EnemyCharacter.h"
#include "Components/GuardAbilityComponent.h"
#include "Components/MovementAbilityComponent.h"
#include "Components/TargetingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/PlayerHUD.h"

AWarriorPlayerController::AWarriorPlayerController()
{
	bReplicates = true;
}

void AWarriorPlayerController::Resume()
{
	TObjectPtr<APlayerHUD> PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (PlayerHUD)
	{
		PlayerHUD->SetPauseOverlay(false);
		bShowMouseCursor = false;
		SetInputMode(GameInputMode);
		SetPause(false);
	}
}

void AWarriorPlayerController::SetMouseSensitivity(float Value)
{
	MouseSensitivity = Value;
}

void AWarriorPlayerController::SwitchInputMode(bool bIsUIMode)
{
	if (bIsUIMode)
	{
		SetInputMode(UIInputMode);
	}
	else
	{
		SetInputMode(GameInputMode);
	}
}

void AWarriorPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//ULocalPlayer의 라이프타임과 동일하며 레벨 이동에 따라 이동된다.
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	check(Context);
	Subsystem->AddMappingContext(Context, 0);

	DefaultMouseCursor = EMouseCursor::Default;
}

void AWarriorPlayerController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);
	PlayerCharacter = GetPawn<APlayerCharacter>();
}

void AWarriorPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AWarriorPlayerController::Attack);

	EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Triggered, this, &AWarriorPlayerController::ExecuteBlock);
	EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &AWarriorPlayerController::CancelBlock);

	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &AWarriorPlayerController::EnableRun);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Completed, this, &AWarriorPlayerController::DisableRun);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Completed, this, &AWarriorPlayerController::Dodge);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AWarriorPlayerController::Jump);

	EnhancedInputComponent->BindAction(LockonAction, ETriggerEvent::Started, this, &AWarriorPlayerController::LockOn);

	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWarriorPlayerController::Look);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWarriorPlayerController::Move);

	EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AWarriorPlayerController::Pause);
}

void AWarriorPlayerController::Attack()
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->Attack();
	}
}

void AWarriorPlayerController::ExecuteBlock()
{
	if (IsValid(PlayerCharacter))
	{
		TObjectPtr<UGuardAbilityComponent> GuardAbility = PlayerCharacter->GetGuardAbility();
		if (IsValid(GuardAbility))
		{
			GuardAbility->ExecuteBlock();
		}
	}
}

void AWarriorPlayerController::CancelBlock()
{
	if (IsValid(PlayerCharacter))
	{
		TObjectPtr<UGuardAbilityComponent> GuardAbility = PlayerCharacter->GetGuardAbility();
		if (IsValid(GuardAbility))
		{
			GuardAbility->CancelBlock();
		}
	}
}

void AWarriorPlayerController::Dodge()
{
	if (IsValid(PlayerCharacter))
	{
		TObjectPtr<UMovementAbilityComponent> MovementAbility = PlayerCharacter->GetMovementAbility();
		if (IsValid(MovementAbility))
		{
			MovementAbility->Dodge();
		}
	}
}

void AWarriorPlayerController::DisableRun()
{
	if (IsValid(PlayerCharacter))
	{
		TObjectPtr<UMovementAbilityComponent> MovementAbility = PlayerCharacter->GetMovementAbility();
		if (IsValid(MovementAbility))
		{
			MovementAbility->DisableRun();
		}
	}
}

void AWarriorPlayerController::EnableRun()
{
	if (IsValid(PlayerCharacter))
	{
		TObjectPtr<UMovementAbilityComponent> MovementAbility = PlayerCharacter->GetMovementAbility();
		if (IsValid(MovementAbility))
		{
			MovementAbility->EnableRun();
		}
	}
}

void AWarriorPlayerController::Jump()
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->Jump();
	}
}

void AWarriorPlayerController::LockOn()
{
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
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>() * MouseSensitivity;
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

void AWarriorPlayerController::Pause()
{
	TObjectPtr<APlayerHUD> PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (PlayerHUD)
	{
		PlayerHUD->SetPauseOverlay(true);
		bShowMouseCursor = true;
		SetInputMode(UIInputMode);
		SetPause(true);
	}
}
