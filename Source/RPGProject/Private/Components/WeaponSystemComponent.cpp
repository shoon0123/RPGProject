// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponSystemComponent.h"
#include "Character/CharacterBase.h"
#include "Data/CharacterBasePDA.h"
#include "Weapon/Weapon.h"

// Sets default values for this component's properties
UWeaponSystemComponent::UWeaponSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UWeaponSystemComponent::DestroyWeapons()
{
	for (TObjectPtr<AWeapon> Weapon : Weapons)
	{
		Weapon->Destroy();
	}
}

TObjectPtr<AWeapon> UWeaponSystemComponent::GetWeapon(uint8 Index)
{
	return Index < Weapons.Num() ? Weapons[Index] : nullptr;
}

void UWeaponSystemComponent::SetWeaponsCollisionDisable()
{
	for (TObjectPtr<AWeapon> Weapon : Weapons)
	{
		Weapon->CollisionEnable(ECollisionEnabled::NoCollision);
	}
}

void UWeaponSystemComponent::SpawnWeapons()
{
	if (IsValid(OwnerCharacter))
	{
		if (IsValid(CharacterInfo))
		{
			for (const FWeaponSocket& WeaponSocket : CharacterInfo->WeaponSocketInfo)
			{
				const FName Socket = WeaponSocket.SocketName;
				const TSubclassOf<class UObject> WeaponClass = WeaponSocket.WeaponType->GeneratedClass;

				TObjectPtr<AWeapon> Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
				Weapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket);
				Weapon->SetOwner(OwnerCharacter);
				Weapons.Add(Weapon);
			}
		}
	}
}

// Called when the game starts
void UWeaponSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}


// Called every frame
void UWeaponSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

