// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GuardAbilityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPROJECT_API UGuardAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGuardAbilityComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Actor Attribute")
	float ParryingPostureDamage = 0.f;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> BlockMontage;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	TObjectPtr<USoundBase> BlockSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	TObjectPtr<USoundBase> ParryingSound;

	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	TObjectPtr<UParticleSystem> BlockParticles;

	void ExecuteBlock();

	void CancelBlock();

	void Block(const FVector& ImpactPoint);

	void ExecuteParrying(const FVector& ImpactPoint, AActor* Hitter);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private :
	UFUNCTION(BlueprintCallable)
	void EnableParrying();

	UFUNCTION(BlueprintCallable)
	void DisableParrying();

	void ExecuteGetPostureDamage(AActor* DamagedActor);

	void PlayBlockSound(const FVector& ImpactPoint);

	void PlayParryingSound(const FVector& ImpactPoint);

	void SpawnBlockParticles(const FVector& ImpactPoint);

	void SpawnParryingParticles(const FVector& ImpactPoint);

	

		
};
