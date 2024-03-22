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

UAnimMontage* APlayerCharacter::GetAttackMontage() const
{
    return AttackMontage;
}

EActionState APlayerCharacter::GetActionState() const
{
    return ActionState;
}

void APlayerCharacter::SetActionState(EActionState OtherActionState)
{
    ActionState = OtherActionState;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay(); 

    check(LeftHandWeapon);
    check(RightHandWeapon);
    FName LeftWeaponSocket(TEXT("hand_lSocket"));
    FName RightWeaponSocket(TEXT("hand_rSocket"));


    TSubclassOf<class UObject> LeftWeaponClass = LeftHandWeapon->GeneratedClass;
    TObjectPtr<AActor> LeftWeapon = GetWorld()->SpawnActor<AActor>(LeftWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
    LeftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftWeaponSocket);

    TSubclassOf<class UObject> RightWeaponClass = RightHandWeapon->GeneratedClass;
    TObjectPtr<AActor> RightWeapon = GetWorld()->SpawnActor<AActor>(RightWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
    RightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightWeaponSocket);
}

void APlayerCharacter::SetSpringArm()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(GetRootComponent());
    SpringArm->AddLocalOffset(FVector3d(0.f, 0.f, 90.f));
    SpringArm->AddLocalRotation(FRotator(-30.f, 0.f, 0.f));
    SpringArm->TargetArmLength = 500.0f;
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
