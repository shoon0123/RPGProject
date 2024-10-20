// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacterBoss.h"
#include "Character/PlayerCharacter.h"
#include "Components/AttributeComponent.h"
#include "Components/TargetingComponent.h"
#include "Data/EnemyCharacterBossPDA.h"
#include "Game/WarriorGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/PlayerHUD.h"
#include "HUD/CombatOverlay.h"
#include "Kismet/GameplayStatics.h"

AEnemyCharacterBoss::AEnemyCharacterBoss()
{
    GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
}

void AEnemyCharacterBoss::Attack()
{
    if (GetCharacterMovement()->IsFalling())
    {
        return;
    }
    if (GetActionState() != EActionState::EAS_Unoccupied && GetActionState() != EActionState::EAS_HitReaction)
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
        TObjectPtr<APlayerCharacter> NewTargetPlayer = Cast<APlayerCharacter>(NewTarget);
        if (IsValid(NewTargetPlayer))
        {
            TObjectPtr<UCombatOverlay> CombatOverlay = NewTargetPlayer->GetCombatOverlay();
            if (IsValid(CombatOverlay))
            {
                CombatOverlay->SetEnemyWidgetVisibility(ESlateVisibility::Visible);
            }
        }
    }
    else
    {
        TObjectPtr<APlayerCharacter> CombatTargetPlayer = Cast<APlayerCharacter>(CombatTarget);
        if (IsValid(CombatTargetPlayer))
        {
            TObjectPtr<UCombatOverlay> CombatOverlay = CombatTargetPlayer->GetCombatOverlay();
            if (IsValid(CombatOverlay))
            {
                CombatOverlay->SetEnemyWidgetVisibility(ESlateVisibility::Hidden);
            }
        }
    }
    Super::SetCombatTarget(NewTarget);
}

bool AEnemyCharacterBoss::CanHitReact()
{
    return GetActionState() != EActionState::EAS_Stunned && GetActionState() != EActionState::EAS_Attacking;
}

void AEnemyCharacterBoss::UpdateHealthBar()
{
    TObjectPtr<APlayerCharacter> TargetPlayer = Cast<APlayerCharacter>(CombatTarget);
    if (IsValid(TargetPlayer))
    {
        TObjectPtr<UCombatOverlay> CombatOverlay = TargetPlayer->GetCombatOverlay();
        if (IsValid(CombatOverlay) && IsValid(Attributes))
        {
            CombatOverlay->SetEnemyHealthPercent(Attributes->GetHealthPercent());
        }
    }
}

void AEnemyCharacterBoss::UpdatePostureBar()
{
    TObjectPtr<APlayerCharacter> TargetPlayer = Cast<APlayerCharacter>(CombatTarget);
    if (IsValid(TargetPlayer))
    {
        TObjectPtr<UCombatOverlay> CombatOverlay = TargetPlayer->GetCombatOverlay();
        if (IsValid(CombatOverlay) && IsValid(Attributes))
        {
            CombatOverlay->SetEnemyPosturePercent(Attributes->GetPosturePercent());
        }
    }
}

void AEnemyCharacterBoss::Die()
{
    TObjectPtr<APlayerCharacter> CombatPlayer = Cast<APlayerCharacter>(CombatTarget);
    if (IsValid(CombatPlayer))
    {
        TObjectPtr<UCombatOverlay> CombatOverlay = CombatPlayer->GetCombatOverlay();
        if (IsValid(CombatOverlay))
        {
            CombatOverlay->SetEnemyWidgetVisibility(ESlateVisibility::Hidden);
        }
    }
    Super::Die();

    TObjectPtr<AWarriorGameModeBase> WarriorGameMode = Cast<AWarriorGameModeBase>(UGameplayStatics::GetGameMode(this));
    WarriorGameMode->EnemyBossDied();
}

void AEnemyCharacterBoss::SetupData()
{
    Super::SetupData();
    TObjectPtr<UEnemyCharacterBossPDA> EnemyCharacterBossInfo = Cast<UEnemyCharacterBossPDA>(CharacterInfo);
    if (IsValid(EnemyCharacterBossInfo))
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

