// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacterBoss.h"
#include "Character/PlayerCharacter.h"
#include "Components/AttributeComponent.h"
#include "HUD/PlayerHUD.h"
#include "HUD/CombatOverlay.h"

void AEnemyCharacterBoss::Attack()
{

    if (IsValid(CombatTarget))
    {
        Super::Attack();

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

void AEnemyCharacterBoss::MeleeAttack()
{
    const int32 Selection = FMath::RandRange(0, 2);
    FName SectionName = FName();
    switch (Selection)
    {
    case 0:
        SectionName = FName("Melee1");
        break;
    case 1:
        SectionName = FName("Melee2");
        break;
    case 2:
        SectionName = FName("Melee3");
        break;
    default:
        break;
    }
    PlayMontageSection(AttackMontage, SectionName);
}

void AEnemyCharacterBoss::MidRangedAttack()
{
    const int32 Selection = FMath::RandRange(0, 2);
    FName SectionName = FName();
    switch (Selection)
    {
    case 0:
        SectionName = FName("MidRanged1");
        break;
    case 1:
        SectionName = FName("MidRanged2");
        break;
    case 2:
        SectionName = FName("MidRanged3");
        break;
    default:
        break;
    }
    PlayMontageSection(AttackMontage, SectionName);
}

void AEnemyCharacterBoss::RangedAttack()
{
    const int32 Selection = FMath::RandRange(0, 2);
    FName SectionName = FName();
    switch (Selection)
    {
    case 0:
        SectionName = FName("Ranged1");
        break;
    case 1:
        SectionName = FName("Ranged1");
        break;
    case 2:
        SectionName = FName("Ranged1");
        break;
    default:
        break;
    }
    PlayMontageSection(AttackMontage, SectionName);
}
