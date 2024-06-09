// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/PlayerCharacter.h"
#include "Character/EnemyCharacter.h"
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
	if (bIsLockingOn)
	{
		if (GetPawn<ACharacter>())
		{
			GetPawn<ACharacter>()->GetCharacterMovement()->bOrientRotationToMovement = true;
			GetPawn<ACharacter>()->GetCharacterMovement()->bUseControllerDesiredRotation = false;
			bIsLockingOn = false;
		}
	}
	else
	{
		if (GetPawn<APlayerCharacter>())
		{
			GetPawn<APlayerCharacter>()->GetCharacterMovement()->bOrientRotationToMovement = false;
			GetPawn<APlayerCharacter>()->GetCharacterMovement()->bUseControllerDesiredRotation = true;
			bIsLockingOn = true;
		}
	}
	
	const FVector PlayerLocation = GetPawn<AActor>()->GetActorLocation();

	TArray<FHitResult> HitResults;
	TArray<AActor*> TargetableActors;

	bool bIsHit = GetWorld()->SweepMultiByChannel(HitResults, PlayerLocation, PlayerLocation, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(2000.f));
	if (bIsHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			DrawDebugSphere(GetWorld(), Hit.GetActor()->GetActorLocation(),
				100.f, 20, FColor::Red, false, 1.f);
		}
	}




	/**
	if (bIsHit)
	{
		for (FHitResult Hit : HitResults)
		{
			if (Hit.Actor.IsValid() == true)
			{
				AEnemyCharacter* HitedAI = Cast<AEnemyCharacter>(Hit.Actor);
				if (IsValid(HitedAI) == true && HitedAI->IsDead() == false)
				{
					if (bInScreenPosition)
					{
						TTuple<FVector2D, bool> ActorScreenPosition = GetScreenPositionOfActor(HitedAI);
						if (IsInViewport(ActorScreenPosition.Get<0>()) == true && ActorScreenPosition.Get<1>() == true)
						{
							// 중복되지 않게 TargetableActors Array에 추가합니다.
							TargetableActors.AddUnique(Cast<AActor>(Hit.Actor));
						}
					}
					else
					{
						// 중복되지 않게 TargetableActors Array에 추가합니다.
						TargetableActors.AddUnique(Cast<AActor>(Hit.Actor));
					}
				}
			}
		}
	}
	*/

}