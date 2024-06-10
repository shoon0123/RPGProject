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

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//ULocalPlayer의 라이프타임과 동일하며 레벨 이동에 따라 이동된다.
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
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
}

void AMyPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetPawn<APlayerCharacter>() && GetPawn<APlayerCharacter>()->GetActionState() != EActionState::EAS_Unoccupied)
	{
		return;
	}

	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);


	if (GetPawn<APawn>())
	{
		GetPawn<APawn>()->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		GetPawn<APawn>()->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AMyPlayerController::Look(const FInputActionValue& InputActionValue)
{
	if (bIsLockingOn)
	{
		return;
	}
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	AddPitchInput(-InputAxisVector.Y);
	AddYawInput(InputAxisVector.X);
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
	if (TObjectPtr<AActor> Target = GetPawn<APlayerCharacter>()->GetTargetingComponent()->FindTarget())
	{
		DrawDebugSphere(GetWorld(), Target->GetActorLocation(), 100.f, 20, FColor::Blue, false, 1.f);

		if (bIsLockingOn)
		{
			if (GetPawn<ACharacter>())
			{
				GetPawn<APlayerCharacter>()->GetTargetingComponent()->SetTarget(nullptr);

				GetPawn<ACharacter>()->GetCharacterMovement()->bOrientRotationToMovement = true;
				GetPawn<ACharacter>()->GetCharacterMovement()->bUseControllerDesiredRotation = false;
				bIsLockingOn = false;
			}
		}
		else
		{
			if (GetPawn<APlayerCharacter>())
			{
				GetPawn<APlayerCharacter>()->GetTargetingComponent()->SetTarget(Target);

				GetPawn<APlayerCharacter>()->GetCharacterMovement()->bOrientRotationToMovement = false;
				GetPawn<APlayerCharacter>()->GetCharacterMovement()->bUseControllerDesiredRotation = true;
				bIsLockingOn = true;
			}
		}

	}

}