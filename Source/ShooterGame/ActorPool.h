// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorPool.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTERGAME_API UActorPool : public UActorComponent
{
	GENERATED_BODY()

	TArray<AActor*> AvailableActors;

protected:

public:	
	// Sets default values for this component's properties
	UActorPool();
	
	void Add(AActor* ActorToAdd);

	AActor* CheckoutActor();
	void ReturnActor(AActor* ActorToReturn);
};
