// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonCharacter.generated.h"

UCLASS()
class SHOOTERGAME_API AThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AShooterWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class AShooterWeapon* Weapon;

	void SpawnWeapon();

public:
	// Sets default values for this character's properties
	AThirdPersonCharacter();

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector GunOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* Camera1P;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	FVector CameraOffset = FVector(12.5f,1.7f,64.f);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	bool bTriggerDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float RemainingHealth = 100;

	UFUNCTION(BlueprintPure, Category = "Gameplay")
	bool IsDead() { return RemainingHealth <= 0.f; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay")
	void Init();

	virtual void UnPossessed() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void PullTrigger();
	
};
