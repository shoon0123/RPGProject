// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/AttributeComponent.h"
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
    SetupTargetingComponent();
    SetupMovementAbility();

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 1000.f, 0.f);
    if (IsValid(MovementAbility))
    {
        GetCharacterMovement()->MaxWalkSpeed = GetMovementAbility()->WalkingSpeed;
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
        bDoNextAttack = true;
    }
    else if(GetActionState() == EActionState::EAS_Unoccupied)
    {
        PlayMontageSection(AttackMontage, FName("Attack1"));
        SetActionState(EActionState::EAS_Attacking);
    }
}

void APlayerCharacter::Block()
{
    if (GetActionState() == EActionState::EAS_Unoccupied && !GetCharacterMovement()->IsFalling())
    {
        if (IsValid(MovementAbility))
        {
            MovementAbility->DisableRun();
        }
        SetActionState(EActionState::EAS_Block);
        PlayMontageSection(BlockMontage, FName("Start"));
    }
}

void APlayerCharacter::BlockCancel()
{
    if (GetActionState() == EActionState::EAS_Block || GetActionState() == EActionState::EAS_Parrying)
    {
        SetActionState(EActionState::EAS_Unoccupied);
        PlayMontageSection(BlockMontage, FName("End"));
    }
}

void APlayerCharacter::DisableParrying()
{
    SetActionState(EActionState::EAS_Block);
}

void APlayerCharacter::EnableParrying()
{
    SetActionState(EActionState::EAS_Parrying);
}

void APlayerCharacter::ExecuteBlock(const FVector& ImpactPoint)
{
    PlayMontageSection(BlockMontage, FName("React"));
    PlayBlockSound(ImpactPoint);
    SpawnBlockParticles(ImpactPoint);
}

void APlayerCharacter::ExecuteGetPostureDamage(AActor* DamagedActor)
{
    IPostureInterface* PostureInterface = Cast<IPostureInterface>(DamagedActor);
    if (PostureInterface)
    {
        PostureInterface->GetPostureDamage(ParryingPostureDamage);
    }
}

void APlayerCharacter::SetupMovementAbility()
{
    MovementAbility = CreateDefaultSubobject<UMovementAbilityComponent>(TEXT("MovementAbility"));
}

void APlayerCharacter::ExecuteParrying(const FVector& ImpactPoint, AActor* Hitter)
{
    const double Angle = GetAngle2DFromForwardVector(ImpactPoint);

    if (Angle < 0)
    {
        PlayMontageSection(BlockMontage, FName("ParryingLeft"));
    }
    else
    {
        PlayMontageSection(BlockMontage, FName("ParryingRight"));
    }

    ExecuteGetPostureDamage(Hitter);
    PlayParryingSound(ImpactPoint);
    SpawnParryingParticles(ImpactPoint);
    DisableParrying();
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
    if (GetActionState() == EActionState::EAS_Block && bIsForward)
    {
        ExecuteBlock(ImpactPoint);
    }
    else if (GetActionState() == EActionState::EAS_Parrying && bIsForward)
    {
        ExecuteParrying(ImpactPoint, Hitter);
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

UMovementAbilityComponent* APlayerCharacter::GetMovementAbility() const
{
    return MovementAbility;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay(); 

    SetupHUD();
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

        ParryingPostureDamage = PlayerCharacterInfo->ParryingPostureDamage;
        BlockMontage = PlayerCharacterInfo->BlockMontage;
        BlockSound = PlayerCharacterInfo->BlockSound;
        ParryingSound = PlayerCharacterInfo->ParryingSound;
        BlockParticles = PlayerCharacterInfo->BlockParticles;
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

void APlayerCharacter::PlayBlockSound(const FVector& ImpactPoint)
{
    check(BlockSound);
    UGameplayStatics::PlaySoundAtLocation(this, BlockSound, ImpactPoint);
}

void APlayerCharacter::PlayParryingSound(const FVector& ImpactPoint)
{
    check(ParryingSound);
    UGameplayStatics::PlaySoundAtLocation(this, ParryingSound, ImpactPoint);
}

void APlayerCharacter::SpawnBlockParticles(const FVector& ImpactPoint)
{
    const FVector ImpactPointNormalVector = (ImpactPoint - GetActorLocation()).GetSafeNormal();
    check(BlockParticles);
    UGameplayStatics::SpawnEmitterAttached(
        BlockParticles,
        GetRootComponent(),
        FName("Block"),
        ImpactPoint,
        ImpactPointNormalVector.Rotation(),
        GetActorScale() * 0.5,
        EAttachLocation::KeepWorldPosition
    );
}

void APlayerCharacter::SpawnParryingParticles(const FVector& ImpactPoint)
{
    const FVector ImpactPointNormalVector = (ImpactPoint - GetActorLocation()).GetSafeNormal();
    check(BlockParticles);
    UGameplayStatics::SpawnEmitterAttached(
        BlockParticles,
        GetRootComponent(),
        FName("Block"),
        ImpactPoint,
        ImpactPointNormalVector.Rotation(),
        GetActorScale()*2,
        EAttachLocation::KeepWorldPosition
    );
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

void APlayerCharacter::SetupTargetingComponent()
{
    TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("Targeting"));
}

void APlayerCharacter::AttackEnd()
{
    if (!bDoNextAttack)
    {
        SetActionState(EActionState::EAS_Unoccupied);
        TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
        check(AnimInstance);
        check(AttackMontage);
        AnimInstance->Montage_Stop(0.5f, AttackMontage);
    }
    bDoNextAttack = false;
}
