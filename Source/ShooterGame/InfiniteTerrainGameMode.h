// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterGameGameMode.h"
#include "InfiniteTerrainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AInfiniteTerrainGameMode : public AShooterGameGameMode
{
	GENERATED_BODY()

	void AddToPool(class ANavMeshBoundsVolume* VolumeToAdd);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool")
	class UActorPool* NavMeshBoundsVolumePool;
	
public:
	AInfiniteTerrainGameMode();
	
	UFUNCTION(BlueprintCallable, Category = "Init")
	void PopulateNavMeshBoundsVolumePool();

	UFUNCTION()
	class UActorPool* GetNavMeshBoundsVolumePool();

};
