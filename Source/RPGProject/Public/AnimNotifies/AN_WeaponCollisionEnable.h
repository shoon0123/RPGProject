// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_WeaponCollisionEnable.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UAN_WeaponCollisionEnable : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	uint8 WeaponIndex;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TEnumAsByte<ECollisionEnabled::Type> CollisionType;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
