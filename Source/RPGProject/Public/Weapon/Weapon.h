// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UBoxComponent;

UCLASS()
class RPGPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	AWeapon();
	TObjectPtr<UBoxComponent> GetWeaponBox() const;
	void EmptyIgnoreActors();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<UBoxComponent> WeaponBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTraceEnd;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 10.f;
};
