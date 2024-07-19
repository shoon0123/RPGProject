// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacterBoss.h"
#include "Character/PlayerCharacter.h"
#include "Components/AttributeComponent.h"
#include "HUD/PlayerHUD.h"
#include "HUD/CombatOverlay.h"

void AEnemyCharacterBoss::SetCombatTarget(ACharacter* Target)
{
    if (TObjectPtr<APlayerCharacter> TargetPlayer = Cast<APlayerCharacter>(Target))
    {
        if (TObjectPtr<UCombatOverlay> CombatOverlay = TargetPlayer->GetCombatOverlay())
        {
            CombatOverlay->SetEnemyWidgetVisibility(ESlateVisibility::Visible);
        }
    }
    else
    {
        if (TObjectPtr<APlayerCharacter> CombatPlayer = Cast<APlayerCharacter>(CombatTarget))
        {
            if (TObjectPtr<UCombatOverlay> CombatOverlay = CombatPlayer->GetCombatOverlay())
            {
                CombatOverlay->SetEnemyWidgetVisibility(ESlateVisibility::Hidden);
            }
        }
    }
    Super::SetCombatTarget(Target);
}

void AEnemyCharacterBoss::UpdateHealthBar()
{
    if (TObjectPtr<APlayerCharacter> TargetPlayer = Cast<APlayerCharacter>(CombatTarget))
    {
        TObjectPtr<UCombatOverlay> CombatOverlay = TargetPlayer->GetCombatOverlay();
        if (CombatOverlay && Attributes)
        {
            CombatOverlay->SetEnemyHealthPercent(Attributes->GetHealthPercent());
        }
    }
}

void AEnemyCharacterBoss::Die()
{
    if (TObjectPtr<APlayerCharacter> CombatPlayer = Cast<APlayerCharacter>(CombatTarget))
    {
        if (TObjectPtr<UCombatOverlay> CombatOverlay = CombatPlayer->GetCombatOverlay())
        {
            CombatOverlay->SetEnemyWidgetVisibility(ESlateVisibility::Hidden);
        }
    }
    Super::Die();
}
