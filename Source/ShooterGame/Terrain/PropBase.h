// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PropBase.generated.h"

UCLASS()
class SHOOTERGAME_API APropBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APropBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawning")
	float BoundsCheckSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawning")
	float MinScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawning")
	float MaxScale;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
