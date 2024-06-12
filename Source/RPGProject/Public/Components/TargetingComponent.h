// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "TargetingComponent.generated.h"

class APlayerCharacter;
class APlayerController;

UCLASS()
class RPGPROJECT_API UTargetingComponent : public USphereComponent
{
	GENERATED_BODY()
	
public:
	UTargetingComponent();

	void ExecuteLockOn();

	void CancelLockOn();

	bool IsLockOn();

protected:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerCharacter;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditAnywhere)
	float TargetableDistance = 3000.f;

	UPROPERTY(EditAnywhere)
	float InterpSpeed = 10.f;

	UPROPERTY(VisibleInstanceOnly)
	TSet<TObjectPtr<AActor>> TargetableActors;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AActor> Target;

	bool bIsLockOn = false;

	void AddTargetableActor(TObjectPtr<AActor> Actor);

	TObjectPtr<AActor> FindTarget();

	void InitializeTargetableActors();

	bool IsTargetable(TObjectPtr<AActor> Actor);

	void RemoveTargetableActor(TObjectPtr<AActor> Actor);

	void SetupCollision();

	void SetTarget(TObjectPtr<AActor> Actor);

	void SetTargetableDistance(float Distance);

	void UpdateCamera();

};
