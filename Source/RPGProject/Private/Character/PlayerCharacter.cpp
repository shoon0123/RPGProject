// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/GuardAbilityComponent.h"
#include "Components/MovementAbilityComponent.h"
#include "Components/TargetingComponent.h"
#include "Data/PlayerCharacterPDA.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/PlayerHUD.h"
#include "HUD/CombatOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Weapon.h"

APlayerCharacter::APlayerCharacter()
{
    SetupSpringArm();
    SetupCamera(); 
    GuardAbility = CreateDefaultSubobject<UGuardAbilityComponent>(TEXT("GuardAbility"));
    MovementAbility = CreateDefaultSubobject<UMovementAbilityComponent>(TEXT("MovementAbility"));
    TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("Targeting"));

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 1000.f, 0.f);
    if (IsValid(MovementAbility))
    {
        GetCharacterMovement()->MaxWalkSpeed = MovementAbility->WalkingSpeed;
    }

    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;
}

void APlayerCharacter::Attack()
{
    if (GetCharacterMovement()->IsFalling())
    {
        return;
    }

    if (GetActionState() == EActionState::EAS_Attacking)
    {
        if (AttackEndTimerHandle.IsValid())
        {
            ComboAttack();
        }
        else {
            bDoNextAttack = true;
        }
    }
    else if(GetActionState() == EActionState::EAS_Unoccupied)
    {
        ComboCount = 0;
        ComboAttack();
    }
}

TObjectPtr<UTargetingComponent> APlayerCharacter::GetTargetingComponent() const
{
    return TargetingComponent;
}

FVector APlayerCharacter::GetSpringArmLocation() const
{
    return SpringArm->GetComponentLocation();
}

void APlayerCharacter::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
    const double Angle = GetAngle2DFromForwardVector(Hitter);
    const bool bIsForward = -90.f < Angle && Angle < 90.f;
    if (GetActionState() == EActionState::EAS_Block && bIsForward && IsValid(GuardAbility))
    {
        GuardAbility->Block(ImpactPoint);
    }
    else if (GetActionState() == EActionState::EAS_Parrying && bIsForward && IsValid(GuardAbility))
    {
        GuardAbility->ExecuteParrying(ImpactPoint, Hitter);
    }
    else
    {
        if (IsValid(MovementAbility))
        {
            MovementAbility->DisableRun();
        }
        Super::GetHit(ImpactPoint, Hitter);
    }
}

TObjectPtr<UCombatOverlay> APlayerCharacter::GetCombatOverlay() const
{
    TObjectPtr<UCombatOverlay> CombatOverlay = nullptr;
    if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController()))
    {
        if (TObjectPtr<APlayerHUD> PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
        {
            CombatOverlay = PlayerHUD->GetCombatOverlay();
        }
    }
    return CombatOverlay;
}

TObjectPtr<UGuardAbilityComponent> APlayerCharacter::GetGuardAbility() const
{
    return GuardAbility;
}

TObjectPtr<UMovementAbilityComponent> APlayerCharacter::GetMovementAbility() const
{
    return MovementAbility;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay(); 

    SetupHUD();
}

void APlayerCharacter::AttackEnd()
{
    if (bDoNextAttack) {
        bDoNextAttack = false;
        ComboAttack();
    }
    else
    {
        Super::AttackEnd();

        TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance && AttackMontage)
        {
            AnimInstance->Montage_Stop(CoolDownToAttack*3, AttackMontage);
        }
    }
}

void APlayerCharacter::AttackCoolDownEnd()
{
    if (GetActionState() == EActionState::EAS_Attacking)
    {
        SetActionState(EActionState::EAS_Unoccupied);
    }
    bDoNextAttack = false;
}


void APlayerCharacter::Die()
{
    Super::Die();

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetTargetingComponent()->CancelLockOn();
}

void APlayerCharacter::SetupData()
{
    Super::SetupData();

    if (TObjectPtr<UPlayerCharacterPDA> PlayerCharacterInfo = Cast<UPlayerCharacterPDA>(CharacterInfo))
    {
        if (IsValid(MovementAbility))
        {
            MovementAbility->DodgingSpeed = PlayerCharacterInfo->DodgingSpeed;
            MovementAbility->WalkingSpeed = PlayerCharacterInfo->WalkingSpeed;
            MovementAbility->RunningSpeed = PlayerCharacterInfo->RunningSpeed;
            MovementAbility->DodgeMontage = PlayerCharacterInfo->DodgeMontage;
        }
        if (IsValid(GuardAbility))
        {
            GuardAbility->ParryingPostureDamage = PlayerCharacterInfo->ParryingPostureDamage;
            GuardAbility->BlockMontage = PlayerCharacterInfo->BlockMontage;
            GuardAbility->BlockSound = PlayerCharacterInfo->BlockSound;
            GuardAbility->ParryingSound = PlayerCharacterInfo->ParryingSound;
            GuardAbility->BlockParticles = PlayerCharacterInfo->BlockParticles;
        }
        
    }
}

void APlayerCharacter::UpdateHealthBar()
{
    if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController()))
    {
        if (TObjectPtr<APlayerHUD> PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
        {
            if (TObjectPtr<UCombatOverlay> CombatOverlay = PlayerHUD->GetCombatOverlay())
            {
                CombatOverlay->SetPlayerHealthPercent(Attributes->GetHealthPercent());
            }
        }
    }
}

void APlayerCharacter::UpdatePostureBar()
{
    if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController()))
    {
        if (TObjectPtr<APlayerHUD> PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
        {
            if (TObjectPtr<UCombatOverlay> CombatOverlay = PlayerHUD->GetCombatOverlay())
            {
                CombatOverlay->SetPlayerPosturePercent(Attributes->GetPosturePercent());
            }
        }
    }
}

void APlayerCharacter::ComboAttack()
{
    PlayMontageSection(AttackMontage, AttackMontageSections[ComboCount]);
    GetWorld()->GetTimerManager().ClearTimer(AttackEndTimerHandle);
    ++ComboCount %= AttackMontageSections.Num();
    SetActionState(EActionState::EAS_Attacking);
}

void APlayerCharacter::SetupSpringArm()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(GetRootComponent());
    SpringArm->AddLocalOffset(FVector3d(0.f, 0.f, 100.f));
    SpringArm->TargetArmLength = 400.0f;
    SpringArm->bUsePawnControlRotation = true;
    SpringArm->bEnableCameraLag = true;
    SpringArm->bInheritPitch = true;
    SpringArm->bInheritYaw = true;
    SpringArm->bInheritRoll = false;
}

void APlayerCharacter::SetupCamera()
{
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void APlayerCharacter::SetupHUD()
{
    TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController());
    if (IsValid(PlayerController))
    {
        TObjectPtr<APlayerHUD> PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
        if (IsValid(PlayerHUD))
        {
            TObjectPtr<UCombatOverlay> CombatOverlay = PlayerHUD->GetCombatOverlay();
            if (IsValid(CombatOverlay))
            {
                CombatOverlay->SetEnemyWidgetVisibility(ESlateVisibility::Hidden);
            }
        }
    }
}

