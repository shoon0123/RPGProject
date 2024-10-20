// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TargetingComponent.h"
#include "Character/EnemyCharacter.h"
#include "Character/PlayerCharacter.h"
#include "Player/WarriorPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UTargetingComponent::UTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTargetingComponent::ExecuteLockOn()
{
	if (!bIsLockOn)
	{
		FindTargetableActors();
		TObjectPtr<AActor> NewTarget = FindTargetInViewport();

		if (IsValid(NewTarget))
		{
			DrawDebugSphere(GetWorld(), NewTarget->GetActorLocation(), 100.f, 20, FColor::Blue, false, 1.f);

			SetTarget(NewTarget);
			SetComponentTickEnabled(true);
			OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
			OwnerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = true;
			bIsLockOn = true;
		}
	}
}

void UTargetingComponent::CancelLockOn()
{
	if (bIsLockOn)
	{
		SetTarget(nullptr);
		TargetableActors.Empty();
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		OwnerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = false;
		SetComponentTickEnabled(false);
		bIsLockOn = false; 
	}
}

void UTargetingComponent::ChangeLockOnTarget(const FVector2D InputVector)
{
	if (!bCanChangeTarget)
	{
		return;
	}
	if (InputVector.Length() < ChangeTargetSensitivity)
	{
		return;
	}

	FindTargetableActors();
	TObjectPtr<AActor> TargetActor = FindClosestTarget(InputVector);
	if (IsValid(TargetActor))
	{
		SetTarget(TargetActor);
	}
	
	
}


bool UTargetingComponent::IsLockOn()
{
	return bIsLockOn;
}

void UTargetingComponent::SetTargetableDistance(float Distance)
{
	TargetableDistance = Distance;
}

TObjectPtr<AActor> UTargetingComponent::FindTargetInViewport()
{
	TObjectPtr<AActor> TargetActor = nullptr;
	if (IsValid(OwnerCharacter))
	{
		TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
		if (IsValid(PlayerController))
		{
			FVector2D ScreenPosition = FVector2D::ZeroVector;
			const FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
			long MinimumDistance = LONG_MAX;

			for (TObjectPtr<AActor> TargetableActor : TargetableActors)
			{
				if (UGameplayStatics::ProjectWorldToScreen(PlayerController, TargetableActor->GetActorLocation(), ScreenPosition))
				{
					const double Distance = FVector2D::Distance(ScreenPosition, ViewportSize * 0.5f);
					if (Distance < MinimumDistance)
					{
						MinimumDistance = Distance;
						TargetActor = TargetableActor;
					}
				}
			}
		}
	}

	return TargetActor;
}

FVector UTargetingComponent::GetCameraLocation()
{
	TObjectPtr<APlayerCharacter> PlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter);

	return IsValid(PlayerCharacter) ? PlayerCharacter->GetSpringArmLocation() : OwnerCharacter->GetActorLocation();
}

float UTargetingComponent::GetTargetDistance()
{
	return OwnerCharacter ? OwnerCharacter->GetDistanceTo(Target) : 0.0f;
}

void UTargetingComponent::SetTarget(TObjectPtr<AActor> Actor)
{
	Target = Cast<ACharacterBase>(Actor); 
	bCanChangeTarget = false;
	GetWorld()->GetTimerManager().SetTimer(ChangeTargetTimerHandle, this, &UTargetingComponent::EnableChangeTarget, ChangeTargetCooldown, false);
	if (IsValid(Target))
	{
		GetWorld()->GetTimerManager().SetTimer(CheckDistanceTimerHandle, this, &UTargetingComponent::CheckTargetDistance, 1.0f, true, 1.0f);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(CheckDistanceTimerHandle);
	}
}

void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);
}

void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateCamera();
}

void UTargetingComponent::AddTargetableActor(TObjectPtr<AActor> Actor)
{
	if (IsTargetable(Actor))
	{
		TargetableActors.Add(Actor);
	}
}

void UTargetingComponent::CheckTargetDistance()
{
	if (GetTargetDistance() > TargetableDistance)
	{
		CancelLockOn();
	}
}

void UTargetingComponent::EnableChangeTarget()
{
	bCanChangeTarget = true;
}

void UTargetingComponent::RemoveTargetableActor(TObjectPtr<AActor> Actor)
{
	if (IsTargetable(Actor))
	{
		TargetableActors.Remove(Actor);
	}
}

void UTargetingComponent::FindTargetableActors()
{
	TargetableActors.Empty();
	TArray<FHitResult> HitResults;
	if (IsValid(OwnerCharacter))
	{
		bool bIsHit = GetWorld()->SweepMultiByChannel(HitResults, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorLocation(), FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(TargetableDistance));
		if (bIsHit)
		{
			for (const FHitResult& Hit : HitResults)
			{
				AddTargetableActor(Hit.GetActor());
			}
		}
	}
}

TObjectPtr<AActor> UTargetingComponent::FindClosestTarget(const FVector2D InputVector)
{
	TObjectPtr<AActor> TargetActor = nullptr;
	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(OwnerCharacter->GetController()))
	{
		FVector2D ScreenPosition = FVector2D::ZeroVector;
		const FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
		long MinimumDistance = LONG_MAX;

		if (FMath::Abs(InputVector.X) >= FMath::Abs(InputVector.Y))
		{
			if (InputVector.X > 0)
			{
				for (TObjectPtr<AActor> TargetableActor : TargetableActors)
				{
					if (TargetableActor != Target && UGameplayStatics::ProjectWorldToScreen(PlayerController, TargetableActor->GetActorLocation(), ScreenPosition))
					{
						const double Distance = ScreenPosition.X - ViewportSize.X * 0.5;

						if (Distance > 0 && Distance < MinimumDistance)
						{
							MinimumDistance = Distance;
							TargetActor = TargetableActor;
						}
					}
				}
			}
			else
			{
				for (TObjectPtr<AActor> TargetableActor : TargetableActors)
				{
					if (TargetableActor != Target && UGameplayStatics::ProjectWorldToScreen(PlayerController, TargetableActor->GetActorLocation(), ScreenPosition))
					{
						const double Distance = ViewportSize.X * 0.5 - ScreenPosition.X;

						if (Distance > 0 && Distance < MinimumDistance)
						{
							MinimumDistance = Distance;
							TargetActor = TargetableActor;
						}
					}
				}
			}
		}
		else
		{
			if (InputVector.Y < 0)
			{
				for (TObjectPtr<AActor> TargetableActor : TargetableActors)
				{
					if (TargetableActor != Target && UGameplayStatics::ProjectWorldToScreen(PlayerController, TargetableActor->GetActorLocation(), ScreenPosition))
					{
						const double Distance = ScreenPosition.Y - ViewportSize.Y * 0.5;

						if (Distance > 0 && Distance < MinimumDistance)
						{
							MinimumDistance = Distance;
							TargetActor = TargetableActor;
						}
					}
				}
			}
			else
			{
				for (TObjectPtr<AActor> TargetableActor : TargetableActors)
				{
					if (TargetableActor != Target && UGameplayStatics::ProjectWorldToScreen(PlayerController, TargetableActor->GetActorLocation(), ScreenPosition))
					{
						const double Distance = ViewportSize.Y * 0.5 - ScreenPosition.Y;

						if (Distance > 0 && Distance < MinimumDistance)
						{
							MinimumDistance = Distance;
							TargetActor = TargetableActor;
						}
					}
				}
			}
		}
	}
	return TargetActor;
}

bool UTargetingComponent::IsTargetable(TObjectPtr<AActor> Actor)
{
	return IsValid(Actor) ? Actor->ActorHasTag(FName("Enemy")) : false;
}

void UTargetingComponent::UpdateCamera()
{
	if (IsValid(Target))
	{
		TObjectPtr<ACharacterBase> TargetCharacter = Cast<ACharacterBase>(Target);

		if (IsValid(TargetCharacter) && !TargetCharacter->IsAlive())
		{
			RemoveTargetableActor(TargetCharacter);
			CancelLockOn(); 
		}
		else if(IsValid(OwnerCharacter))
		{
			TObjectPtr<AController> Controller = OwnerCharacter->GetController();
			if (IsValid(Controller))
			{
				const FRotator ControllerRotator = Controller->GetControlRotation();
				const FVector CameraLocation = GetCameraLocation();
				const FVector LookAtLocation = (Target->GetActorLocation() + OwnerCharacter->GetActorLocation()) / 2;
				const FRotator LookAtTarget = UKismetMathLibrary::FindLookAtRotation(CameraLocation, LookAtLocation);
				const FRotator RInterpToRotator = FMath::RInterpTo(ControllerRotator, LookAtTarget,
					GetWorld()->GetDeltaSeconds(), InterpSpeed);
				Controller->SetControlRotation(RInterpToRotator);
			}
		}
	}
}