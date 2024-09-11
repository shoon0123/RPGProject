// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterBaseComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API UAttributeComponent : public UCharacterBaseComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

	float GetHealthPercent();

	float GetPosturePercent();

	void ReceiveDamage(float Damage);

	void ReceivePostureDamage(float PostureDamage);

	void RecoverHealth(float Amount);

	void RecoverPosture(float Amount);

	void SetPostureZero();

	void SetHealth(float Amount);

	void SetMaxHealth(float Amount);

	void SetPosture(float Amount);

	void SetMaxPosture(float Amount);

	void SetRecoveryAmountPerSec(float Amount);

	bool IsAlive();

	bool IsPostureBroken();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health = 100;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Posture = 0;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxPosture = 100;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float RecoveryAmountPerSec = 1;

	void RecoverPerSec();
};
