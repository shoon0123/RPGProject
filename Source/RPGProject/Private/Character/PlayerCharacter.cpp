// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/TargetingComponent.h"
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

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 1000.f, 0.f);
    GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;

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
        SetActionState(EActionState::EAS_Block);
        DisableRun();
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

void APlayerCharacter::Dodge()
{
    if (GetActionState() == EActionState::EAS_Unoccupied && !GetCharacterMovement()->IsFalling())
    {
        SetActionState(EActionState::EAS_Dodge);
        PlayMontageSection(DodgeMontage, FName("Dodge"));

        if (GetCharacterMovement()->Velocity.IsZero())
        {
            LaunchCharacter(GetActorForwardVector() * DodgingSpeed, true, true);
        }
        else
        {
            LaunchCharacter(GetCharacterMovement()->Velocity.GetSafeNormal2D() * DodgingSpeed, true, true);
        }
    }
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

void APlayerCharacter::ExecuteParrying(const FVector& ImpactPoint)
{
    if (bParryingLeft)
    {
        PlayMontageSection(BlockMontage, FName("ParryingLeft"));
    }
    else
    {
        PlayMontageSection(BlockMontage, FName("ParryingRight"));
    }
    bParryingLeft = !bParryingLeft;

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

void APlayerCharacter::EnableRun()
{
    GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
}

void APlayerCharacter::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
    if (GetActionState() == EActionState::EAS_Block)
    {
        ExecuteBlock(ImpactPoint);
    }
    else if (GetActionState() == EActionState::EAS_Parrying)
    {
        ExecuteParrying(ImpactPoint);
    }
    else
    {
        Super::GetHit(ImpactPoint, Hitter);
    }
}

void APlayerCharacter::DisableRun()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay(); 

    SpawnWeapons();
}

void APlayerCharacter::DestroyWeapon()
{
    if (LeftHandWeapon)
    {
        LeftHandWeapon->Destroy();
    }
    if (RightHandWeapon)
    {
        RightHandWeapon->Destroy();
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

void APlayerCharacter::SetupTargetingComponent()
{
    TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("Targeting"));
    TargetingComponent->SetupAttachment(GetRootComponent());
}

void APlayerCharacter::SpawnWeapons()
{
    check(LeftHandWeaponType);
    check(RightHandWeaponType);
    FName LeftWeaponSocket(TEXT("hand_lSocket"));
    FName RightWeaponSocket(TEXT("hand_rSocket"));

    TSubclassOf<class UObject> LeftWeaponClass = LeftHandWeaponType->GeneratedClass;
    LeftHandWeapon = GetWorld()->SpawnActor<AWeapon>(LeftWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
    LeftHandWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftWeaponSocket);
    LeftHandWeapon->SetOwner(this);
    Weapons.Add(LeftHandWeapon);

    TSubclassOf<class UObject> RightWeaponClass = RightHandWeaponType->GeneratedClass;
    RightHandWeapon = GetWorld()->SpawnActor<AWeapon>(RightWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
    RightHandWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightWeaponSocket);
    RightHandWeapon->SetOwner(this);
    Weapons.Add(RightHandWeapon);
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

void APlayerCharacter::DodgeEnd()
{
    SetActionState(EActionState::EAS_Unoccupied);
    DisableRun();
}
