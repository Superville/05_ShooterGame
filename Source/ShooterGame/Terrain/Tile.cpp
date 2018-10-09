// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "DrawDebugHelpers.h"


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

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto NumSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (int i = 0; i < NumSpawn; i++)
	{
		int SpawnIdx = FMath::RandRange(0,ToSpawn.Num()-1);

		FVector RandPosition = FMath::RandPointInBox(FBox(MinPosition, MaxPosition));
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ToSpawn[SpawnIdx], RandPosition, FRotator(0,FMath::RandRange(-180.f,180.f),0), ActorSpawnParams);
		SpawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

//		DrawDebugBox(GetWorld(), RandPosition, FVector(5, 5, 5), FColor::Red, true);
	}
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
