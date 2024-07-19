// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"
#include "AI/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AttributeComponent.h"
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
	EnemyAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	EnemyAIController->RunBehaviorTree(BehaviorTree);
	EnemyAIController->GetBlackboardComponent()->SetValueAsFloat(FName("DetectionRange"), DetectionRange);
}

void AEnemyCharacter::SetActionState(EActionState OtherActionState)
{
	Super::SetActionState(OtherActionState);
	EnemyAIController->GetBlackboardComponent()->SetValueAsEnum(FName("ActionState"), (uint8)OtherActionState);
}

void AEnemyCharacter::Attack()
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}
	if (GetActionState() == EActionState::EAS_Unoccupied)
	{
		const int32 Selection = FMath::RandRange(0, 3);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		case 2:
			SectionName = FName("Attack3");
			break;
		default:
			break;
		}
		PlayMontageSection(AttackMontage, SectionName);
		SetActionState(EActionState::EAS_Attacking);
	}
}

float AEnemyCharacter::GetDetectionRange() const
{
	return DetectionRange;
}

void AEnemyCharacter::SetCombatTarget(ACharacter* Target)
{
	CombatTarget = Target;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyCharacter::Die()
{
	Super::Die();
	SetLifeSpan(10.f);
}

void AEnemyCharacter::AttackEnd()
{
	SetActionState(EActionState::EAS_Unoccupied);
}
