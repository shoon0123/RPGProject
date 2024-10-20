// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/WeaponSystemComponent.h"
#include "Data/CharacterBasePDA.h"
#include "Components/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Weapon.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SetupCollision();

	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->FallingLateralFriction = 2.0f;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	WeaponSystem = CreateDefaultSubobject<UWeaponSystemComponent>(TEXT("WeaponSystem"));
}

void ACharacterBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetupData();
}

EActionState ACharacterBase::GetActionState() const
{
	return ActionState;
}

TObjectPtr<UCharacterBasePDA> ACharacterBase::GetCharacterInfo() const
{
	return CharacterInfo;
}

double ACharacterBase::GetAngle2DFromForwardVector(AActor* OtherActor) const
{
	return IsValid(OtherActor) ? GetAngle2DFromForwardVector(OtherActor->GetActorLocation()) : 0.f ;
}

double ACharacterBase::GetAngle2DFromForwardVector(const FVector& Location) const
{
	const FVector Vector = Location - GetActorLocation();
	const FVector Vector2D = Vector.GetSafeNormal2D();
	const FVector ForwardVector2D = GetActorForwardVector().GetSafeNormal2D();
	const double CosTheta = FVector::DotProduct(ForwardVector2D, Vector2D);
	const double Theta = FMath::Acos(CosTheta);
	double Angle = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(ForwardVector2D, Vector2D);

	return (CrossProduct.Z >= 0) ? Angle : -Angle;
}

void ACharacterBase::SetActionState(EActionState OtherActionState)
{
	ActionState = OtherActionState;
}

void ACharacterBase::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
	if (IsValid(WeaponSystem))
	{
		WeaponSystem->SetWeaponsCollisionDisable();
	}
}

void ACharacterBase::GetPostureDamage(const float PostureDamage)
{
	if (IsValid(Attributes) && GetActionState() != EActionState::EAS_Parrying)
	{
		Attributes->ReceivePostureDamage(PostureDamage);
		UpdatePostureBar();

		if (Attributes->IsPostureBroken() && GetActionState() != EActionState::EAS_Stunned)
		{
			Stunned();
		}
	}
}

void ACharacterBase::Stunned()
{
	SetActionState(EActionState::EAS_Stunned);

	PlayMontageSection(StunnedMontage, "Default");
}

TObjectPtr<UWeaponSystemComponent> ACharacterBase::GetWeaponSystem() const
{
	return WeaponSystem;
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (GetActionState() == EActionState::EAS_Block || GetActionState() == EActionState::EAS_Parrying)
	{
		const double Angle = GetAngle2DFromForwardVector(DamageCauser);
		if (-90.f < Angle && Angle < 90.f)
		{
			return 0;
		}
	}

	if (IsValid(Attributes))
	{
		if (GetActionState() == EActionState::EAS_Stunned)
		{
			DamageAmount *= 2;
		}
		Attributes->ReceiveDamage(DamageAmount);
		UpdateHealthBar();

		if (IsAlive())
		{
			if (CanHitReact())
			{
				DirectionalHitReact(DamageCauser);
			}
		}
		else
		{
			Die();
		}
	}

	return DamageAmount;
}

void ACharacterBase::Destroyed()
{
	Super::Destroyed();
	if (IsValid(WeaponSystem))
	{
		WeaponSystem->DestroyWeapons();
	}
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(WeaponSystem))
	{
		WeaponSystem->SpawnWeapons();
	}
	if (IsValid(Attributes))
	{
		Attributes->SetHealth(CharacterInfo->MaxHealth);
		Attributes->SetPosture(0.f);
	}
}

void ACharacterBase::AttackEnd()
{
	GetWorld()->GetTimerManager().SetTimer(AttackEndTimerHandle, this, &ACharacterBase::AttackCoolDownEnd, CoolDownToAttack, false);
}

void ACharacterBase::AttackCoolDownEnd()
{
	if (GetActionState() == EActionState::EAS_Attacking)
	{
		SetActionState(EActionState::EAS_Unoccupied);
	}
}

bool ACharacterBase::CanHitReact()
{
	return GetActionState() != EActionState::EAS_Stunned;
}

void ACharacterBase::Die()
{
	SetActionState(EActionState::EAS_Dead);
	if (!DeathMontageSections.IsEmpty())
	{
		const int32 Selection = FMath::RandRange(0, DeathMontageSections.Num() - 1);
		PlayMontageSection(DeathMontage, DeathMontageSections[Selection]); 
		TEnumAsByte<EDeathPose> Pose(Selection + 1);
		DeathPose = Pose;
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACharacterBase::DirectionalHitReact(AActor* Hitter)
{
	SetActionState(EActionState::EAS_HitReaction);

	double Angle = GetAngle2DFromForwardVector(Hitter);

	FName Section("Back");

	if (Angle >= -45.f && Angle < 45.f)
	{
		Section = FName("Front");
	}
	else if (Angle >= -135.f && Angle < -45.f)
	{
		Section = FName("Left");
	}
	else if (Angle >= 45.f && Angle < 135.f)
	{
		Section = FName("Right");
	}

	PlayMontageSection(HitReactMontage, Section);
}

void ACharacterBase::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

void ACharacterBase::HitReactEnd()
{
	SetActionState(EActionState::EAS_Unoccupied);
}

bool ACharacterBase::IsAlive()
{
	return IsValid(Attributes) && Attributes->IsAlive();
}

void ACharacterBase::PlayHitSound(const FVector& ImpactPoint)
{
	check(HitSound);
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
}

void ACharacterBase::SetupCollision()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ACharacterBase::SetupData()
{
	if (IsValid(CharacterInfo))
	{
		if (IsValid(Attributes))
		{
			Attributes->SetHealth(CharacterInfo->MaxHealth);
			Attributes->SetMaxHealth(CharacterInfo->MaxHealth);
			Attributes->SetPosture(0.f);
			Attributes->SetMaxPosture(CharacterInfo->MaxPosture);
			Attributes->SetRecoveryAmountPerSec(CharacterInfo->RecoveryAmountPerSec);
		}
		CoolDownToAttack = CharacterInfo->CoolDownToAttack;

		AttackMontage = CharacterInfo->AttackMontage;
		DeathMontage = CharacterInfo->DeathMontage;
		HitReactMontage = CharacterInfo->HitReactMontage;
		StunnedMontage = CharacterInfo->StunnedMontage;
		
		AttackMontageSections = CharacterInfo->AttackMontageSections;
		DeathMontageSections = CharacterInfo->DeathMontageSections;
		HitReactMontageSections = CharacterInfo->HitReactMontageSections;

		HitSound = CharacterInfo->HitSound;
		HitParticles = CharacterInfo->HitParticles;
	}
}

void ACharacterBase::SpawnHitParticles(const FVector& ImpactPoint)
{
	const FVector ImpactPointNormalVector = (ImpactPoint - GetActorLocation()).GetSafeNormal();
	check(HitParticles);

	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		HitParticles,
		FTransform(ImpactPointNormalVector.Rotation(), ImpactPoint, GetActorScale() * 2)
	);
}

void ACharacterBase::StunnedEnd()
{
	if (IsValid(Attributes))
	{
		Attributes->SetPostureZero();
		UpdatePostureBar();
	}
	SetActionState(EActionState::EAS_Unoccupied);
}
