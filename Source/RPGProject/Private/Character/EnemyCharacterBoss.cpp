// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacterBoss.h"
#include "Character/PlayerCharacter.h"
#include "Components/AttributeComponent.h"
#include "Components/TargetingComponent.h"
#include "Data/EnemyCharacterBossPDA.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/PlayerHUD.h"
#include "HUD/CombatOverlay.h"

AEnemyCharacterBoss::AEnemyCharacterBoss()
{
}

void AEnemyCharacterBoss::Attack()
{
    if (GetCharacterMovement()->IsFalling())
    {
        return;
    }
    if (GetActionState() != EActionState::EAS_Unoccupied)
    {
        return;
    }
    if (IsValid(CombatTarget))
    {
        SetActionState(EActionState::EAS_Attacking);

        const float Distance = GetDistanceTo(CombatTarget);

        if (Distance > RangedAttackDistance)
        {
            RangedAttack();
        }
        else if (Distance > MeleeAttackDistance)
        {
            MidRangedAttack();
        }
        else
        {
            MeleeAttack();
        }
    }
}

void AEnemyCharacterBoss::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
    PlayHitSound(ImpactPoint);
    SpawnHitParticles(ImpactPoint);
    UpdateHealthBar();
    if (!IsAlive())
    {
        Die();
    }
}

void AEnemyCharacterBoss::SetCombatTarget(ACharacterBase* NewTarget)
{
    if (IsValid(NewTarget))
    {
        if (TObjectPtr<APlayerCharacter> NewTargetPlayer = Cast<APlayerCharacter>(NewTarget))
        {
            if (TObjectPtr<UCombatOverlay> CombatOverlay = NewTargetPlayer->GetCombatOverlay())
            {
                CombatOverlay->SetEnemyWidgetVisibility(ESlateVisibility::Visible);
            }
        }
    }
    else
    {
        if (TObjectPtr<APlayerCharacter> CombatTargetPlayer = Cast<APlayerCharacter>(CombatTarget))
        {
            if (TObjectPtr<UCombatOverlay> CombatOverlay = CombatTargetPlayer->GetCombatOverlay())
            {
                CombatOverlay->SetEnemyWidgetVisibility(ESlateVisibility::Hidden);
            }
        }
    }
    Super::SetCombatTarget(NewTarget);
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

void AEnemyCharacterBoss::UpdatePostureBar()
{
    if (TObjectPtr<APlayerCharacter> TargetPlayer = Cast<APlayerCharacter>(CombatTarget))
    {
        TObjectPtr<UCombatOverlay> CombatOverlay = TargetPlayer->GetCombatOverlay();
        if (CombatOverlay && Attributes)
        {
            CombatOverlay->SetEnemyPosturePercent(Attributes->GetPosturePercent());
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

void AEnemyCharacterBoss::SetupData()
{
    Super::SetupData();

    if (TObjectPtr<UEnemyCharacterBossPDA> EnemyCharacterBossInfo = Cast<UEnemyCharacterBossPDA>(CharacterInfo))
    {
        MeleeAttackDistance = EnemyCharacterBossInfo->MeleeAttackDistance;
        RangedAttackDistance = EnemyCharacterBossInfo->RangedAttackDistance;
        MidRangedAttackMontageSections = EnemyCharacterBossInfo->MidRangedAttackMontageSections;
        RangedAttackMontageSections = EnemyCharacterBossInfo->RangedAttackMontageSections;
    }
}

void AEnemyCharacterBoss::MeleeAttack()
{
    if (!AttackMontageSections.IsEmpty())
    {
        const int32 Selection = FMath::RandRange(0, AttackMontageSections.Num() - 1);
        PlayMontageSection(AttackMontage, AttackMontageSections[Selection]);
    }
}

void AEnemyCharacterBoss::MidRangedAttack()
{
    if (!MidRangedAttackMontageSections.IsEmpty())
    {
        const int32 Selection = FMath::RandRange(0, MidRangedAttackMontageSections.Num() - 1);
        PlayMontageSection(AttackMontage, MidRangedAttackMontageSections[Selection]);
    }
}

void AEnemyCharacterBoss::RangedAttack()
{
    if (!RangedAttackMontageSections.IsEmpty())
    {
        const int32 Selection = FMath::RandRange(0, RangedAttackMontageSections.Num() - 1);
        PlayMontageSection(AttackMontage, RangedAttackMontageSections[Selection]);
    }
}

