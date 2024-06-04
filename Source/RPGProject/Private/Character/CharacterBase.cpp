// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Weapon/Weapon.h"

ACharacterBase::ACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetUpCollision();

	GetCharacterMovement()->FallingLateralFriction = 2.0f;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
}

EActionState ACharacterBase::GetActionState() const
{
	return ActionState;
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
	check(Attributes);
	if (Attributes->IsAlive())
	{
		SetActionState(EActionState::EAS_HitReaction);
		DirectionalHitReact(Hitter);
		HealthBarWidget->SetVisibility(true);
	}
	else
	{
		SetActionState(EActionState::EAS_Dead);
		Die();
	}
	

	check(HitSound);
	UGameplayStatics::PlaySoundAtLocation(
		this,
		HitSound,
		ImpactPoint
	);

	const FVector ImpactPointVector = (ImpactPoint - GetActorLocation()).GetSafeNormal();
	const double CosTheta = FVector::DotProduct(GetActorForwardVector(), ImpactPointVector);

	check(HitParticles);
	UGameplayStatics::SpawnEmitterAttached(
		HitParticles,
		GetRootComponent(),
		FName("Hit"),
		ImpactPoint,
		ImpactPointVector.Rotation(),
		GetActorScale() * 2,
		EAttachLocation::KeepWorldPosition
	);

	check(HealthBarWidget);
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	check(Attributes);
	Attributes->ReceiveDamage(DamageAmount);
	check(HealthBarWidget);
	HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());

	return DamageAmount;
}

void ACharacterBase::Destroyed()
{
	Super::Destroyed();
	DestroyWeapon();
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	check(HealthBarWidget);
	HealthBarWidget->SetVisibility(false);
}

void ACharacterBase::Die()
{
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
	SetLifeSpan(5.f);
	HealthBarWidget->SetVisibility(false);
}

void ACharacterBase::DirectionalHitReact(const AActor* Hitter)
{
	const FVector HitterVector = Hitter->GetActorLocation() - GetActorLocation();
	const FVector HitterVectorXY = FVector(HitterVector.X, HitterVector.Y, 0).GetSafeNormal();
	
	GetCharacterMovement()->AddImpulse(-HitterVectorXY * 100000);

	const double CosTheta = FVector::DotProduct(GetActorForwardVector(), HitterVectorXY);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);
	const FVector CrossProduct = FVector::CrossProduct(GetActorForwardVector(), HitterVectorXY);

	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("Back");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("Front");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("Left");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("Right");
	}

	PlayMontageSection(HitReactMontage, Section);
}

void ACharacterBase::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	check(Montage);
	TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
	check(AnimInstance);
	AnimInstance->Montage_Play(Montage);
	AnimInstance->Montage_JumpToSection(SectionName, Montage);
}

void ACharacterBase::HitReactEnd()
{
	SetActionState(EActionState::EAS_Unoccupied);
}

void ACharacterBase::SetUpCollision()
{
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
