// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"
#include "Components/BoxComponent.h"
#include "Interaction/HitInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(Mesh);

	
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap); 
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	FVector A(GetWeaponBox() ->GetUnscaledBoxExtent());

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetWeaponBox());
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetWeaponBox());
}

TObjectPtr<UBoxComponent> AWeapon::GetWeaponBox() const
{
	return TObjectPtr<UBoxComponent>(WeaponBox);
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
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	ActorsToIgnore.Append(GetOwner()->Children);
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start, 
		End,
		FVector(5.f, 5.f, 5.f),
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		BoxHit,
		true
	);
	if (BoxHit.GetActor())
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		if (HitInterface)
		{
			HitInterface->GetHit(BoxHit.ImpactPoint, GetOwner());
		}
		IgnoreActors.AddUnique(BoxHit.GetActor());

		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			Damage,
			GetOwner()->GetInstigatorController(),
			this,
			UDamageType::StaticClass()
			);

	}
}