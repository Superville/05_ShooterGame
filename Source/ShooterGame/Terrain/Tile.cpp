// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "ActorPool.h"
#include "InfiniteTerrainGameMode.h"
#include "PropBase.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/GameFramework/Pawn.h"
#include "AI/ThirdPersonCharacter.h"
#include "DrawDebugHelpers.h"

#define ECC_Spawn ECC_GameTraceChannel2 

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NavigationBoundsOffset = FVector(2000, 0, 0);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

//	UE_LOG(LogTemp, Warning, TEXT("[%s] ATile::EndPlay"), *GetName());

	if (NavMeshBoundsVolumePool != nullptr && NavMeshBoundsRef != nullptr)
	{
//		UE_LOG(LogTemp, Warning, TEXT("[%s] ATile::EndPlay - Return Actor"), *GetName(), *NavMeshBoundsRef->GetName());

		NavMeshBoundsVolumePool->ReturnActor(NavMeshBoundsRef);
	}

	for (int i = 0; i < PropsToCleanup.Num(); i++)
	{
		PropsToCleanup[i]->Destroy();
	}
	PropsToCleanup.Empty();
}

void ATile::SetActorPool(class UActorPool* Pool)
{
	NavMeshBoundsVolumePool = Pool;

//	UE_LOG(LogTemp, Warning, TEXT("[%s] ATile::SetActorPool - %s"), *GetName(), *NavMeshBoundsVolumePool->GetName());

	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	if (NavMeshBoundsVolumePool == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] ATile::PositionNavMeshBoundsVolume - No pool!"), *GetName());
		return;
	}

	NavMeshBoundsRef = NavMeshBoundsVolumePool->CheckoutActor();
	if (NavMeshBoundsRef == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] ATile::PositionNavMeshBoundsVolume - Not enough actors in pool."), *GetName());
		return;
	}

//	UE_LOG(LogTemp, Warning, TEXT("[%s] ATile::PositionNavMeshBoundsVolume - checked out: %s"), *GetName(), *NavMeshBoundsRef->GetName());

	NavMeshBoundsRef->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	GetWorld()->GetNavigationSystem()->Build();
}

void ATile::SpawnActorsWithinTile(FVector MinPosition, FVector MaxPosition, TArray<TSubclassOf<APropBase>> ToSpawn, int MinSpawn, int MaxSpawn)
{
	if (ToSpawn.Num() <= 0)
		return;

	auto NumSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	FSpawnParams SpawnParams;
	for (int i = 0; i < NumSpawn; i++)
	{
		int SpawnIdx = FMath::RandRange(0,ToSpawn.Num()-1);
		auto SpawnProp = ToSpawn[SpawnIdx];
		
		auto DefaultObj = Cast<APropBase>(SpawnProp->GetDefaultObject());
		SpawnParams.Rotation = FMath::RandRange(-180, 180);
		SpawnParams.Scale = FMath::RandRange(DefaultObj->MinScale, DefaultObj->MaxScale);

		float BoundsCheckSize = DefaultObj->BoundsCheckSize * SpawnParams.Scale;
		if (FindEmptyLocation(SpawnParams.Location, BoundsCheckSize, MinPosition, MaxPosition))
		{
			//UE_LOG(LogTemp, Warning, TEXT("[%s] SpawnActorsWithinTile - %s %s"), *GetName(), *SpawnProp->GetName(), *SpawnParams.ToString());

			PlaceActor(SpawnProp, SpawnParams);
		}
	}
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnParams& SpawnParams)
{
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnParams.Location);
	SpawnTransform.SetRotation(FRotator(0, SpawnParams.Rotation, 0).Quaternion());
	SpawnTransform.SetScale3D(FVector(SpawnParams.Scale));

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ToSpawn, SpawnTransform, ActorSpawnParams);
	if (SpawnedActor != nullptr)
	{
		SpawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		SpawnedActor->SetActorScale3D(FVector(SpawnParams.Scale));

		PropsToCleanup.Add(SpawnedActor);
	}
}

void ATile::SpawnAIWithinTile(FVector MinPosition, FVector MaxPosition, TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn)
{
	auto NumSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	FSpawnParams SpawnParams;
	for (int i = 0; i < NumSpawn; i++)
	{
		if (FindEmptyLocation(SpawnParams.Location, 100.f, MinPosition, MaxPosition))
		{
			PlaceAI(ToSpawn, SpawnParams);
		}
	}
}

void ATile::PlaceAI(TSubclassOf<APawn> ToSpawn, FSpawnParams& SpawnParams)
{
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnParams.Location);
	SpawnTransform.SetRotation(FRotator(0, SpawnParams.Rotation, 0).Quaternion());

	APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(ToSpawn, SpawnTransform);
	if (SpawnedPawn != nullptr)
	{
		SpawnedPawn->SpawnDefaultController();
		SpawnedPawn->Tags.Add(FName("Guard"));
		auto TPC = Cast<AThirdPersonCharacter>(SpawnedPawn);
		if (TPC != nullptr)
		{
			TPC->Init();
		}
		PropsToCleanup.Add(SpawnedPawn);
	}
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

	//DrawDebugSphere(GetWorld(), Location, Radius, 32, bHit ? FColor::Red : FColor::Green, true);
	return !bHit;
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
