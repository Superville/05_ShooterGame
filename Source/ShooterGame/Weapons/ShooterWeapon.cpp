// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterWeapon.h"
#include "Weapons/Projectiles/ShooterGameProjectile.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter/FirstPersonCharacter.h"
#include "AI/ThirdPersonCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "Runtime/Engine/Classes/Components/PawnNoiseEmitterComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Hearing.h"

// Sets default values
AShooterWeapon::AShooterWeapon()
{
	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AShooterWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanFire())
	{
		OnFire();
	}
}

bool AShooterWeapon::CanFire()
{
	auto APA = GetAttachParentActor();
	if (APA != nullptr)
	{
		auto TPC = Cast<AThirdPersonCharacter>(APA);
		if (TPC != nullptr)
		{
			return (TPC->bTriggerDown && NextFireTime <= GetWorld()->GetTimeSeconds());
		}
	}

	return false;
}

void AShooterWeapon::OnFire()
{
	//APawn* P = Cast<APawn>(GetRootComponent()->GetAttachmentRootActor());
	APawn* P = Cast<APawn>(GetAttachParentActor());

	// try and fire a projectile
	if (ProjectileClass != NULL && P != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = P->GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : P->GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AShooterGameProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	AThirdPersonCharacter* TPC = Cast<AThirdPersonCharacter>(GetAttachParentActor());
	// try and play a firing animation if specified
	if (FireAnimation != NULL && TPC != nullptr && TPC->Mesh1P != nullptr && TPC->Mesh1P->GetAnimInstance() != NULL)
	{
		TPC->Mesh1P->GetAnimInstance()->Montage_Play(FireAnimation, 1.f);
	}

	if (FireRate > 0.f)
	{
		NextFireTime = GetWorld()->GetTimeSeconds() + 1.f / FireRate;
	}

	UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), 1.f, P);
}
