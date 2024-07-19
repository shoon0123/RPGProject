// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CharacterTypes.h"

AEnemyAIController::AEnemyAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard);
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComponent);
	GetBlackboardComponent()->SetValueAsEnum(FName("ActionState"), (uint8)EActionState::EAS_Unoccupied);
	GetBlackboardComponent()->SetValueAsEnum(FName("TargetState"), (uint8)EActionState::EAS_Unoccupied);
}
