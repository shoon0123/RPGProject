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

double ACharacterBase::GetAngleXYFromForwardVector(FVector Vector) const
{
	const FVector VectorXY = FVector(Vector.X, Vector.Y, 0);
	const double CosTheta = FVector::DotProduct(GetActorForwardVector(), VectorXY);
	double Angle = FMath::RadiansToDegrees(FMath::Acos(CosTheta));
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
	UpdateHealthBar();
	if (IsAlive())
	{
		DirectionalHitReact(Hitter);
	}
	else
	{
		Die();
	}
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	check(Attributes);
	Attributes->ReceiveDamage(DamageAmount);

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

void ACharacterBase::DirectionalHitReact(const AActor* Hitter)
{
	SetActionState(EActionState::EAS_HitReaction);

	const FVector HitterVector = Hitter->GetActorLocation() - GetActorLocation();
	
	double Angle = GetAngleXYFromForwardVector(HitterVector);

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
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

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
