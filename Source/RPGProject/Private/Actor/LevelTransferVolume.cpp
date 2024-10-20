// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/LevelTransferVolume.h"
#include "Character/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ALevelTransferVolume::ALevelTransferVolume()
{
	PrimaryActorTick.bCanEverTick = true;

	TransferVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransferVolume"));
	SetRootComponent(TransferVolume);
	TransferVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TransferVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	TransferVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void ALevelTransferVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	TObjectPtr<APlayerCharacter> PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (IsValid(PlayerCharacter))
	{
		UGameplayStatics::OpenLevel(this, TransferLevelName);
	}
}

