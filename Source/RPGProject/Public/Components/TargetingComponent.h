// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "TargetingComponent.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UTargetingComponent : public USphereComponent
{
	GENERATED_BODY()
	
public:
	UTargetingComponent();

	void SetTargetableDistance(float Distance);

	TObjectPtr<AActor> FindTarget();

	float GetScreenPosition();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditAnywhere)
	float TargetableDistance = 3000.f;

	UPROPERTY(VisibleInstanceOnly)
	TSet<TObjectPtr<AActor>> TargetableActors;

	void AddTargetableActor(TObjectPtr<AActor> Actor);

	void RemoveTargetableActor(TObjectPtr<AActor> Actor);

	void InitializeTargetableActors();

	bool IsTargetable(TObjectPtr<AActor> Target);

	void SetCollision();
};
