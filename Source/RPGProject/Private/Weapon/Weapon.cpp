// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"
#include "Character/CharacterBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/HitInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	SetupMesh();
	SetupWeaponBox();
	SetupBoxTraceStartEnd();
}

void AWeapon::CollisionEnable(ECollisionEnabled::Type CollisionEnabled)
{
	EmptyIgnoreActors();
	WeaponBox->SetCollisionEnabled(CollisionEnabled);
}

void AWeapon::EmptyIgnoreActors()
{
	IgnoreActors.Empty();
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FHitResult BoxHit;
	BoxTrace(BoxHit);

	TObjectPtr<ACharacterBase> HittedCharacter = Cast<ACharacterBase>(BoxHit.GetActor());
	const FName TargetTag = GetTargetTag();
	if (HittedCharacter && HittedCharacter->ActorHasTag(TargetTag))
	{
		if (HittedCharacter->GetActionState() != EActionState::EAS_Parrying)
		{
			AddImpulse(HittedCharacter);
			if (HittedCharacter->GetActionState() != EActionState::EAS_Block)
			{
				UGameplayStatics::ApplyDamage(HittedCharacter, Damage, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
			}
		}
		ExecuteGetHit(BoxHit);
	}
}

void AWeapon::AddImpulse(ACharacter* HittedCharacter)
{
	if (GetOwner())
	{
		const FVector HitVector = HittedCharacter->GetActorLocation() - GetOwner()->GetActorLocation();
		HittedCharacter->GetCharacterMovement()->AddImpulse(HitVector.GetSafeNormal2D() * Impulse * 10000);
	}
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	ActorsToIgnore.Append(GetOwner()->Children);
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}
	

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(WeaponBox->GetScaledBoxExtent().X, WeaponBox->GetScaledBoxExtent().Y, 1.f),
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		BoxHit,
		true
	);
	IgnoreActors.AddUnique(BoxHit.GetActor());
}

void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface)
	{
		HitInterface->GetHit(BoxHit.ImpactPoint, GetOwner());
	}
}

FName AWeapon::GetTargetTag()
{
	return GetOwner()->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");
}

void AWeapon::SetupMesh()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(Mesh);
}

void AWeapon::SetupWeaponBox()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void AWeapon::SetupBoxTraceStartEnd()
{
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(WeaponBox);
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(WeaponBox);
}
