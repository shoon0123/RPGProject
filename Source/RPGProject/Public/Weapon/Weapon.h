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

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Impulse = 100000.f;

	void AddImpulse(ACharacter* HittedCharacter);

	void BoxTrace(FHitResult& BoxHit);

	UFUNCTION(BlueprintCallable)
	void CollisionEnable(ECollisionEnabled::Type CollisionEnabled);

	void ExecuteGetHit(FHitResult& BoxHit);

	FName GetTargetTag();

	void SetupMesh();

	void SetupWeaponBox();

	void SetupBoxTraceStartEnd();

};
