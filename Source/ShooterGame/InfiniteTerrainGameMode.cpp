// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameMode.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "ActorPool.h"

AInfiniteTerrainGameMode::AInfiniteTerrainGameMode() : Super()
{
	NavMeshBoundsVolumePool = CreateDefaultSubobject<UActorPool>(TEXT("NavMeshVolumePool"));
}

UActorPool* AInfiniteTerrainGameMode::GetNavMeshBoundsVolumePool()
{
	return NavMeshBoundsVolumePool;
}

void AInfiniteTerrainGameMode::AddToPool(class ANavMeshBoundsVolume* VolumeToAdd)
{
	NavMeshBoundsVolumePool->Add(VolumeToAdd);
//	UE_LOG(LogTemp, Warning, TEXT("AInfiniteTerrainGameMode::AddToPool - %s"), *VolumeToAdd->GetName());
}

void AInfiniteTerrainGameMode::PopulateNavMeshBoundsVolumePool()
{
	auto BoundsItr = TActorIterator<ANavMeshBoundsVolume>(GetWorld());
	while (BoundsItr)
	{
		AddToPool(*BoundsItr);
		++BoundsItr;
	}
}