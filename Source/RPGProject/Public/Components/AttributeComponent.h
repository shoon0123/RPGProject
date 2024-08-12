// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetHealthPercent();

	float GetPosturePercent();

	void ReceiveDamage(float Damage);

	void ReceivePostureDamage(float PostureDamage);

	void SetPostureZero();

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
};
