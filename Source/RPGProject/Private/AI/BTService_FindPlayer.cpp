// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Character/EnemyCharacter.h"
#include "Character/PlayerCharacter.h"
#include "HUD/CombatOverlay.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	
	TObjectPtr<AEnemyCharacter> OwningPawn = Cast<AEnemyCharacter>(AIOwner->GetPawn());
	if (!OwningPawn->IsAlive())
	{
		return;
	}

	TArray<AActor*> ActorWithTag;
	
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, FName("Player"), ActorWithTag);

	TObjectPtr<ACharacterBase> TargetActor = ActorWithTag.IsEmpty() ? nullptr : Cast<ACharacterBase>(ActorWithTag[0]);
	float Distance = OwningPawn->GetDistanceTo(TargetActor);
	OwningPawn->SetCombatTarget(TargetActor);

	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, Distance);
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, TargetActor);

	if (TargetActor)
	{
		UBTFunctionLibrary::SetBlackboardValueAsEnum(this, TargetStateSelector, (uint8)TargetActor->GetActionState());
		
		if (OwningPawn->GetDetectionRange() < Distance)
		{
			OwningPawn->SetCombatTarget(nullptr);
		}
	}
}