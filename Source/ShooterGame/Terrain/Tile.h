// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT(BlueprintType)
struct FSpawnParams
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Scale;

	FString ToString() { return FString::Printf(TEXT("Location: %s Rotation: %f Scale: %f"), *Location.ToString(), Rotation, Scale); }
};

UCLASS()
class SHOOTERGAME_API ATile : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnActorsWithinTile(FVector MinPosition, FVector MaxPosition, TArray<TSubclassOf<class APropBase>> ToSpawn, int MinSpawn, int MaxSpawn);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnAIWithinTile(FVector MinPosition, FVector MaxPosition, TSubclassOf<class APawn> ToSpawn, int MinSpawn, int MaxSpawn);
	
	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetActorPool(class UActorPool* Pool);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationBoundsOffset;

	
private:
	bool CanSpawnAt(FVector Location, float Radius);
	bool FindEmptyLocation(FVector& out_EmptyLocation, float Radius, FVector MinPosition, FVector MaxPosition);
	void PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnParams& SpawnParams);
	void PlaceAI(TSubclassOf<class APawn> ToSpawn, FSpawnParams& SpawnParams);
	void PositionNavMeshBoundsVolume();

	UPROPERTY()
	class UActorPool* NavMeshBoundsVolumePool;

	UPROPERTY()
	AActor* NavMeshBoundsRef = nullptr;

	UPROPERTY()
	TArray<AActor*> PropsToCleanup;



};
