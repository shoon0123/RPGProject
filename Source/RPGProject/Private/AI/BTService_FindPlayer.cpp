// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	
	APawn* OwningPawn = AIOwner->GetPawn();

	//const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	const FName TargetTag = FName("Player");
	TArray<AActor*> ActorWithTag;
	
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorWithTag);


	AActor* TargetActor = ActorWithTag.IsEmpty() ? nullptr : ActorWithTag[0];
	float Distance = OwningPawn->GetDistanceTo(TargetActor);
	Distance = 100.f;
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, TargetActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, Distance);


}