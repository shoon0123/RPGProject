// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovementAbilityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API UMovementAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMovementAbilityComponent();

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditAnywhere, Category = "Movement Attributes")
	float DodgingSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement Attributes")
	float WalkingSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement Attributes")
	float RunningSpeed;

	void Dodge();

	void EnableRun();

	void DisableRun();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UFUNCTION(BlueprintCallable)
	void DodgeEnd();
		
};
