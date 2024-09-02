// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "CharacterAnimInstance.generated.h"

class ACharacterBase;
class AWarriorPlayerController;
class APlayerCharacter;
class UCharacterMovementComponent;
class UMovementAbilityComponent;
class UWeaponSystemComponent;
enum class EActionState : uint8;

UCLASS()
class RPGPROJECT_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	virtual void NativeBeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacterBase> Character;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float MovementAngle;

	UPROPERTY(BlueprintReadOnly, Category = Weapon)
	TObjectPtr<UWeaponSystemComponent> WeaponSystem;
};
