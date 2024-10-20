// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AN_WeaponCollisionEnable.h"
#include "Character/CharacterBase.h"
#include "Components/WeaponSystemComponent.h"
#include "Weapon/Weapon.h"

void UAN_WeaponCollisionEnable::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp)
	{
		TObjectPtr<ACharacterBase> OwnerCharacter = Cast<ACharacterBase>(MeshComp->GetOwner());
		if (IsValid(OwnerCharacter))
		{
			TObjectPtr<AWeapon> Weapon = OwnerCharacter->GetWeaponSystem()->GetWeapon(WeaponIndex);
			if (IsValid(Weapon))
			{
				Weapon->CollisionEnable(CollisionType);
			}
		}
	}
}
