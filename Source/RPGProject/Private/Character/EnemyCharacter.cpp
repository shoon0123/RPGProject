// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"
#include "AI/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/weapon.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 1000.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (!HasAuthority()) return;
	EnemyAIController = Cast<AEnemyAIController>(NewController);
	EnemyAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	EnemyAIController->RunBehaviorTree(BehaviorTree);
	EnemyAIController->GetBlackboardComponent()->SetValueAsEnum(FName("ActionState"), (uint8)EActionState::EAS_Unoccupied);
}

void AEnemyCharacter::SetActionState(EActionState OtherActionState)
{
	Super::SetActionState(OtherActionState);
	EnemyAIController->GetBlackboardComponent()->SetValueAsEnum(FName("ActionState"), (uint8)OtherActionState);
}


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
}

void AEnemyCharacter::DestroyWeapon()
{
	if (Weapon)
	{
		Weapon->Destroy();
	}
}

void AEnemyCharacter::SpawnWeapon()
{
	FName WeaponSocket(TEXT("RightHandSocket"));
	check(WeaponType);
	TSubclassOf<class UObject> WeaponClass = WeaponType->GeneratedClass;
	Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	Weapon->SetOwner(this);
}
