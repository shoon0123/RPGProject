// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GuardAbilityComponent.h"
#include "Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UGuardAbilityComponent::UGuardAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UGuardAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGuardAbilityComponent::Block(const FVector& ImpactPoint)
{
	TObjectPtr<ACharacterBase> OwnerCharacter = Cast<ACharacterBase>(GetOwner());
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->PlayMontageSection(BlockMontage, FName("React"));
		PlayBlockSound(ImpactPoint);
		SpawnBlockParticles(ImpactPoint);
	}
}

void UGuardAbilityComponent::EnableParrying()
{
	TObjectPtr<ACharacterBase> OwnerCharacter = Cast<ACharacterBase>(GetOwner());
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->SetActionState(EActionState::EAS_Parrying);
	}
}

void UGuardAbilityComponent::DisableParrying()
{
	TObjectPtr<ACharacterBase> OwnerCharacter = Cast<ACharacterBase>(GetOwner());
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->SetActionState(EActionState::EAS_Block);
	}
}

void UGuardAbilityComponent::ExecuteParrying(const FVector& ImpactPoint, AActor* Hitter)
{
	TObjectPtr<ACharacterBase> OwnerCharacter = GetOwner<ACharacterBase>();;
	
	if (IsValid(OwnerCharacter))
	{
		const double Angle = OwnerCharacter->GetAngle2DFromForwardVector(ImpactPoint);

		if (Angle < 0)
		{
			OwnerCharacter->PlayMontageSection(BlockMontage, FName("ParryingLeft"));
		}
		else
		{
			OwnerCharacter->PlayMontageSection(BlockMontage, FName("ParryingRight"));
		}

		ExecuteGetPostureDamage(Hitter);
		PlayParryingSound(ImpactPoint);
		SpawnParryingParticles(ImpactPoint);
		DisableParrying();
	}
	
}

void UGuardAbilityComponent::ExecuteGetPostureDamage(AActor* DamagedActor)
{
	IPostureInterface* PostureInterface = Cast<IPostureInterface>(DamagedActor);
	if (PostureInterface)
	{
		PostureInterface->GetPostureDamage(ParryingPostureDamage);
	}
}

void UGuardAbilityComponent::PlayBlockSound(const FVector& ImpactPoint)
{
	if (BlockSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BlockSound, ImpactPoint);
	}
}

void UGuardAbilityComponent::PlayParryingSound(const FVector& ImpactPoint)
{
	if (ParryingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ParryingSound, ImpactPoint);
	}
}

void UGuardAbilityComponent::SpawnBlockParticles(const FVector& ImpactPoint)
{
	TObjectPtr<ACharacterBase> OwnerCharacter = Cast<ACharacterBase>(GetOwner());
	if (IsValid(OwnerCharacter))
	{
		const FVector ImpactPointNormalVector = (ImpactPoint - OwnerCharacter->GetActorLocation()).GetSafeNormal();
		check(BlockParticles);
		UGameplayStatics::SpawnEmitterAttached(
			BlockParticles,
			OwnerCharacter->GetRootComponent(),
			FName("Block"),
			ImpactPoint,
			ImpactPointNormalVector.Rotation(),
			OwnerCharacter->GetActorScale() * 0.5,
			EAttachLocation::KeepWorldPosition
		);
	}
}

void UGuardAbilityComponent::SpawnParryingParticles(const FVector& ImpactPoint)
{
	TObjectPtr<ACharacterBase> OwnerCharacter = Cast<ACharacterBase>(GetOwner());
	if (IsValid(OwnerCharacter))
	{
		const FVector ImpactPointNormalVector = (ImpactPoint - OwnerCharacter->GetActorLocation()).GetSafeNormal();
		check(BlockParticles);
		UGameplayStatics::SpawnEmitterAttached(
			BlockParticles,
			OwnerCharacter->GetRootComponent(),
			FName("Block"),
			ImpactPoint,
			ImpactPointNormalVector.Rotation(),
			OwnerCharacter->GetActorScale() * 2,
			EAttachLocation::KeepWorldPosition
		);
	}
}


// Called every frame
void UGuardAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGuardAbilityComponent::ExecuteBlock()
{
	TObjectPtr<ACharacterBase> OwnerCharacter = Cast<ACharacterBase>(GetOwner());
	if (IsValid(OwnerCharacter))
	{
		if (OwnerCharacter->GetActionState() == EActionState::EAS_Unoccupied && !OwnerCharacter->GetCharacterMovement()->IsFalling())
		{
			OwnerCharacter->SetActionState(EActionState::EAS_Block);
			OwnerCharacter->PlayMontageSection(BlockMontage, FName("Start"));
		}
	}
	
}

void UGuardAbilityComponent::CancelBlock()
{
	TObjectPtr<ACharacterBase> OwnerCharacter = Cast<ACharacterBase>(GetOwner());
	if (IsValid(OwnerCharacter))
	{
		if (OwnerCharacter->GetActionState() == EActionState::EAS_Block || OwnerCharacter->GetActionState() == EActionState::EAS_Parrying)
		{
			OwnerCharacter->SetActionState(EActionState::EAS_Unoccupied);
			OwnerCharacter->PlayMontageSection(BlockMontage, FName("End"));
		}
	}
}
