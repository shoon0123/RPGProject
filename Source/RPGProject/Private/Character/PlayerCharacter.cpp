// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/Weapon.h"

APlayerCharacter::APlayerCharacter()
{
    SetSpringArm();
    SetCamera();

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 1000.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;
}

void APlayerCharacter::Attack()
{
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

void APlayerCharacter::SetSpringArm()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(GetRootComponent());
    SpringArm->AddLocalOffset(FVector3d(0.f, 0.f, 70.f));
    SpringArm->TargetArmLength = 400.0f;
    SpringArm->bUsePawnControlRotation = true;
    SpringArm->bEnableCameraLag = true;
    SpringArm->bInheritPitch = true;
    SpringArm->bInheritYaw = true;
    SpringArm->bInheritRoll = false;
}

void APlayerCharacter::SetCamera()
{
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
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