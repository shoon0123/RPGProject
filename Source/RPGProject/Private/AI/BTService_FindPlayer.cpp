// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	
	APawn* OwningPawn = AIOwner->GetPawn();

	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorWithTag);

	for (AActor* Actor : ActorWithTag)
	{
		
	}
}
