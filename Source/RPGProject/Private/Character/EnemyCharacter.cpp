// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"
#include "AI/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AttributeComponent.h"
#include "Data/EnemyCharacterPDA.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/weapon.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 300.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (!HasAuthority()) return;

	EnemyAIController = Cast<AEnemyAIController>(NewController);
	if (IsValid(EnemyAIController))
	{
		EnemyAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		EnemyAIController->RunBehaviorTree(BehaviorTree);
		EnemyAIController->GetBlackboardComponent()->SetValueAsFloat(FName("DetectionRange"), DetectionRange);
	}
}

void AEnemyCharacter::SetActionState(EActionState OtherActionState)
{
	Super::SetActionState(OtherActionState);
	EnemyAIController->GetBlackboardComponent()->SetValueAsEnum(FName("ActionState"), (uint8)OtherActionState);
}

void AEnemyCharacter::Attack()
{
	SetActionState(EActionState::EAS_Attacking);
}

float AEnemyCharacter::GetDetectionRange() const
{
	return DetectionRange;
}

TObjectPtr<ACharacterBase> AEnemyCharacter::GetCombatTarget() const
{
	return CombatTarget;
}

void AEnemyCharacter::SetCombatTarget(ACharacterBase* NewTarget)
{
	CombatTarget = NewTarget;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyCharacter::Die()
{
	Super::Die();
	SetLifeSpan(30.f);
}

void AEnemyCharacter::SetupData()
{
	Super::SetupData();
	TObjectPtr<UEnemyCharacterPDA> EnemyCharacterInfo = Cast<UEnemyCharacterPDA>(CharacterInfo);
	if (IsValid(EnemyCharacterInfo))
	{
		BehaviorTree = EnemyCharacterInfo->BehaviorTree;
		DetectionRange = EnemyCharacterInfo->DetectionRange;
	}
}

void AEnemyCharacter::AttackEnd()
{
	SetActionState(EActionState::EAS_Unoccupied);
}
