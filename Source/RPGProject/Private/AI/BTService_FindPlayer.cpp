// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	
	APawn* OwningPawn = AIOwner->GetPawn();

	TArray<AActor*> ActorWithTag;
	
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, FName("Player"), ActorWithTag);

	AActor* TargetActor = ActorWithTag.IsEmpty() ? nullptr : ActorWithTag[0];
	float Distance = OwningPawn->GetDistanceTo(TargetActor);
	//GEngine->AddOnScreenDebugMessage(-1, .5f, FColor::Orange, ActorWithTag[0]->GetName());

	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, Distance);
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, TargetActor);


}