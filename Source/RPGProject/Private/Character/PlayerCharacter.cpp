// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/TargetingComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
        PlayMontageSection(BlockMontage, FName("Start"));
    }
}

void APlayerCharacter::BlockCancel()
{
    if (GetActionState() == EActionState::EAS_Block)
    {
        SetActionState(EActionState::EAS_Unoccupied);
        PlayMontageSection(BlockMontage, FName("End"));
    }
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

void APlayerCharacter::SetupSpringArm()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(GetRootComponent());
    SpringArm->AddLocalOffset(FVector3d(0.f, 0.f, 90.f));
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

    TSubclassOf<class UObject> RightWeaponClass = RightHandWeaponType->GeneratedClass;
    RightHandWeapon = GetWorld()->SpawnActor<AWeapon>(RightWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
    RightHandWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightWeaponSocket);
    RightHandWeapon->SetOwner(this);
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
