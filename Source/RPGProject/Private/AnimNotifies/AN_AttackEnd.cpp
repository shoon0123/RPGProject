// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AN_AttackEnd.h"
#include "Character/CharacterBase.h"

void UAN_AttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp)
	{
		TObjectPtr<ACharacterBase> OwnerCharacter = Cast<ACharacterBase>(MeshComp->GetOwner());
		if (IsValid(OwnerCharacter))
		{
			OwnerCharacter->AttackEnd();
		}
	}
}
