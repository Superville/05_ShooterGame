// Fill out your copyright notice in the Description page of Project Settings.

#include "PropBase.h"


// Sets default values
APropBase::APropBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoundsCheckSize = 100.f;
	MinScale = 1.f;
	MaxScale = 1.f;
}

// Called when the game starts or when spawned
void APropBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APropBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

