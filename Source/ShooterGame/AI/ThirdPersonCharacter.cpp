// Fill out your copyright notice in the Description page of Project Settings.

#include "ThirdPersonCharacter.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/ChildActorComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Weapons/ShooterWeapon.h"


// Sets default values
AThirdPersonCharacter::AThirdPersonCharacter()
{
	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera1P = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera1P"));
	Camera1P->SetupAttachment(GetRootComponent());
	Camera1P->SetRelativeLocation(CameraOffset);
	Camera1P->bUsePawnControlRotation = true;


	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetupAttachment(Camera1P);
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
}

// Called when the game starts or when spawned
void AThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();

	if (InputComponent != nullptr)
	{
		InputComponent->BindAction("Fire", IE_Pressed, this, &AThirdPersonCharacter::PullTrigger);
	}
}

void AThirdPersonCharacter::Destroyed()
{
	Super::Destroyed();

	if (Weapon != nullptr)
	{
		Weapon->Destroy();
	}
}


void AThirdPersonCharacter::SpawnWeapon()
{
	if (!ensure(WeaponClass)) { return; }

	Weapon = GetWorld()->SpawnActor<AShooterWeapon>(WeaponClass, Mesh1P->GetSocketLocation("GripPoint"), Mesh1P->GetSocketRotation("GripPoint"));
	if (Weapon != nullptr)
	{
		if (IsPlayerControlled())
		{
			Weapon->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		}
		else
		{
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		}
		Weapon->AnimInstanceFP = Mesh1P->GetAnimInstance();
		Weapon->AnimInstanceTP = GetMesh()->GetAnimInstance();
	}
}

void AThirdPersonCharacter::UnPossessed()
{
	Super::UnPossessed();

	if (Weapon != nullptr)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
}

// Called every frame
void AThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AThirdPersonCharacter::PullTrigger()
{
	if (Weapon != nullptr)
	{
		Weapon->OnFire();
	}
}

