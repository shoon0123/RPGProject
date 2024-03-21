// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	AWeapon();
private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = Weapon)
	TObjectPtr<USkeletalMeshComponent> Mesh;
};
