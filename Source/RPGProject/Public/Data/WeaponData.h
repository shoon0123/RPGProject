#pragma once

#include "CoreMinimal.h"
#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct RPGPROJECT_API FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Impulse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PostureDamage;

};