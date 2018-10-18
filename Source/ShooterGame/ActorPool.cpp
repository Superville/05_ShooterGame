// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"


// Sets default values for this component's properties
UActorPool::UActorPool()
{
}

void UActorPool::Add(AActor* ActorToAdd)
{
	if (ActorToAdd == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] UActorPool::Add - tried to add null actor"), *GetName());
		return;
	}

	AvailableActors.Push(ActorToAdd);

//	UE_LOG(LogTemp, Warning, TEXT("[%s] UActorPool::Add - %s / %d"), *GetName(), *ActorToAdd->GetName(), AvailableActors.Num());
}

AActor* UActorPool::CheckoutActor()
{
	if (AvailableActors.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] UActorPool::CheckoutActor - NONE AVAILABLE"), *GetName());
		return nullptr;
	}
	return AvailableActors.Pop();
}

void UActorPool::ReturnActor(AActor* ActorToReturn)
{
//	UE_LOG(LogTemp, Warning, TEXT("[%s] UActorPool::ReturnActor"), *GetName());

	Add(ActorToReturn);
}
