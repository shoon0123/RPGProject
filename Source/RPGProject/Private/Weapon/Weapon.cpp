// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"

AWeapon::AWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	if (Mesh)
	{
		Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RootComponent = Mesh;
	}
}