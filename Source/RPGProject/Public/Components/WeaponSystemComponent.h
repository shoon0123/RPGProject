// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterBaseComponent.h"
#include "WeaponSystemComponent.generated.h"

class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API UWeaponSystemComponent : public UCharacterBaseComponent
{
	GENERATED_BODY()

public:	
	UWeaponSystemComponent();

	void DestroyWeapons();

	TObjectPtr<AWeapon> GetWeapon(uint8 Index);

	void SetWeaponsCollisionDisable();

	void SpawnWeapons();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<TObjectPtr<AWeapon>> Weapons;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
