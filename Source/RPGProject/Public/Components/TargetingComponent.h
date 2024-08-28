// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"

class ACharacterBase;
class APlayerCharacter;
class APlayerController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPGPROJECT_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UTargetingComponent();

	void ExecuteLockOn();

	void CancelLockOn();

	void ChangeLockOnTarget(const FVector2D InputVector);

	bool IsLockOn();

protected:
	UPROPERTY()
	TObjectPtr<ACharacterBase> OwnerCharacter;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	float TargetableDistance = 3000.f;

	UPROPERTY(EditAnywhere)
	float InterpSpeed = 10.f;

	UPROPERTY(EditAnywhere)
	float ChangeTargetSensitivity = 2.f;

	UPROPERTY(EditAnywhere)
	float ChangeTargetCooldown = 0.5f;

	UPROPERTY(VisibleInstanceOnly)
	TSet<TObjectPtr<AActor>> TargetableActors;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AActor> Target;

	UPROPERTY(VisibleInstanceOnly)
	bool bIsLockOn = false;

	float LastTimeSetTarget = 0.f;

	void AddTargetableActor(TObjectPtr<AActor> Actor);

	void FindTargetableActors();

	TObjectPtr<AActor> FindTargetInViewport();

	FVector GetCameraLocation();

	bool IsTargetable(TObjectPtr<AActor> Actor);

	void RemoveTargetableActor(TObjectPtr<AActor> Actor);

	void SetTarget(TObjectPtr<AActor> Actor);

	void SetTargetableDistance(float Distance);

	void UpdateCamera();

};
