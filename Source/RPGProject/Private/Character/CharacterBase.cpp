// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/BoxComponent.h"
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
}

EActionState ACharacterBase::GetActionState() const
{
	return ActionState;
}

void ACharacterBase::DestroyWeapons()
{
	for (TObjectPtr<AWeapon> Weapon : Weapons)
	{
		Weapon->Destroy();
	}
}

double ACharacterBase::GetAngleXYFromForwardVector(AActor* Actor) const
{
	return GetAngleXYFromForwardVector(Actor->GetActorLocation());
}

double ACharacterBase::GetAngleXYFromForwardVector(const FVector& Location) const
{
	const FVector Vector = Location - GetActorLocation();
	const FVector VectorXY = Vector.GetSafeNormal2D();
	const double CosTheta = FVector::DotProduct(GetActorForwardVector(), VectorXY);
	double Theta = FMath::Acos(CosTheta);
	double Angle = FMath::RadiansToDegrees(Theta);
	const FVector CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VectorXY);

	if (CrossProduct.Z < 0)
	{
		Angle *= -1.f;
	}

	return Angle;
}

void ACharacterBase::SetActionState(EActionState OtherActionState)
{
	ActionState = OtherActionState;
}

UAnimMontage* ACharacterBase::GetAttackMontage() const
{
	return AttackMontage;
}

void ACharacterBase::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
	SetWeaponsCollisionDisable();
	if (IsAlive())
	{
		if (GetActionState() != EActionState::EAS_Stunned)
		{
			DirectionalHitReact(Hitter);
		}
	}
	else
	{
		Die();
	}
}

void ACharacterBase::GetPostureDamage(const float PostureDamage)
{
	if (Attributes)
	{
		Attributes->ReceivePostureDamage(PostureDamage);
		UpdatePostureBar();

		if (Attributes->IsPostureBroken() && GetActionState() != EActionState::EAS_Stunned)
		{
			GetStunned();
		}
	}
}

void ACharacterBase::GetStunned()
{
	SetActionState(EActionState::EAS_Stunned);

	PlayMontageSection(StunnedMontage, "Default");
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && GetActionState() != EActionState::EAS_Block)
	{
		if (GetActionState() == EActionState::EAS_Stunned)
		{
			DamageAmount *= 2;
		}
		Attributes->ReceiveDamage(DamageAmount);
		UpdateHealthBar();
	}

	return DamageAmount;
}

void ACharacterBase::Destroyed()
{
	Super::Destroyed();
	DestroyWeapons();
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapons();
}

void ACharacterBase::Die()
{
	SetActionState(EActionState::EAS_Dead);
	const int32 Selection = FMath::RandRange(0, 3);
	FName SectionName = FName();
	switch (Selection)
	{
	case 0:
		SectionName = FName("Death1");
		DeathPose = EDeathPose::EDP_Death1;
		break;
	case 1:
		SectionName = FName("Death2");
		DeathPose = EDeathPose::EDP_Death2;
		break;
	case 2:
		SectionName = FName("Death3");
		DeathPose = EDeathPose::EDP_Death3;
		break;
	case 3:
		SectionName = FName("Death4");
		DeathPose = EDeathPose::EDP_Death4;
		break;
	default:
		break;
	}
	PlayMontageSection(DeathMontage, SectionName);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACharacterBase::DirectionalHitReact(AActor* Hitter)
{
	SetActionState(EActionState::EAS_HitReaction);

	double Angle = GetAngleXYFromForwardVector(Hitter);

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
	check(Montage);
	if (TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance())
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
	return Attributes && Attributes->IsAlive();
}

void ACharacterBase::PlayHitSound(const FVector& ImpactPoint)
{
	check(HitSound);
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
}

void ACharacterBase::SetupCollision()
{
	//GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	//GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACharacterBase::SetWeaponsCollisionDisable()
{
	for (TObjectPtr<AWeapon> Weapon : Weapons)
	{
		Weapon->CollisionEnable(ECollisionEnabled::NoCollision);
	}
}

void ACharacterBase::SpawnHitParticles(const FVector& ImpactPoint)
{
	const FVector ImpactPointNormalVector = (ImpactPoint - GetActorLocation()).GetSafeNormal();
	check(HitParticles);
	UGameplayStatics::SpawnEmitterAttached(
		HitParticles,
		GetRootComponent(),
		FName("Hit"),
		ImpactPoint,
		ImpactPointNormalVector.Rotation(),
		GetActorScale() * 2,
		EAttachLocation::KeepWorldPosition
	);
}

void ACharacterBase::SpawnWeapons()
{
	for (const TObjectPtr<UBlueprint> WeaponType : WeaponTypes)
	{
		const FName Socket = *WeaponTypeSocketMap.Find(WeaponType);
		TSubclassOf<class UObject> WeaponClass = WeaponType->GeneratedClass;

		TObjectPtr<AWeapon> Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
		
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket);
		Weapon->SetOwner(this);
		Weapons.Add(Weapon);
	}
}

void ACharacterBase::StunnedEnd()
{
	if (Attributes)
	{
		Attributes->SetPostureZero();
		UpdatePostureBar();
	}
	SetActionState(EActionState::EAS_Unoccupied);
}
