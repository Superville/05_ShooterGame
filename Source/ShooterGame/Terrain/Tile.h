// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class SHOOTERGAME_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnActorsWithinTile(FVector MinPosition, FVector MaxPosition, TArray<TSubclassOf<AActor>> ToSpawn, int MinSpawn, int MaxSpawn);

private:
	bool CanSpawnAt(FVector Location, float Radius);
	bool FindEmptyLocation(FVector& out_EmptyLocation, float Radius, FVector MinPosition, FVector MaxPosition);
	void PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint);
};
