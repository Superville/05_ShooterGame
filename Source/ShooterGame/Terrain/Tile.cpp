// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "DrawDebugHelpers.h"

#define ECC_Spawn ECC_GameTraceChannel2 

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::SpawnActorsWithinTile(FVector MinPosition, FVector MaxPosition, TArray<TSubclassOf<AActor>> ToSpawn, int MinSpawn, int MaxSpawn)
{
	if (ToSpawn.Num() <= 0)
		return;

	auto NumSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	FVector SpawnPoint;
	for (int i = 0; i < NumSpawn; i++)
	{
		int SpawnIdx = FMath::RandRange(0,ToSpawn.Num()-1);
		if (FindEmptyLocation(SpawnPoint, 350.f, MinPosition, MaxPosition))
		{
			PlaceActor(ToSpawn[SpawnIdx], SpawnPoint);
		}
	}
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint)
{
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ToSpawn, SpawnPoint, FRotator(0, FMath::RandRange(-180.f, 180.f), 0), ActorSpawnParams);
	SpawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
}

#define MAX_ATTEMPTS 100
bool ATile::FindEmptyLocation(FVector& out_EmptyLocation, float Radius, FVector MinPosition, FVector MaxPosition)
{
	for (int i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector RandLocation = FMath::RandPointInBox(FBox(MinPosition, MaxPosition));
		if (CanSpawnAt(RandLocation, Radius))
		{
			out_EmptyLocation = RandLocation;
			return true;
		}
	}
	return false;
}


bool ATile::CanSpawnAt(FVector Location, float Radius)
{
	FHitResult HitResult;
	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, Location, Location, FQuat::Identity, ECC_Spawn, FCollisionShape::MakeSphere(Radius));

	//test
	//DrawDebugSphere(GetWorld(), Location, Radius, 32, bHit ? FColor::Red : FColor::Green, true);
	return !bHit;
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
