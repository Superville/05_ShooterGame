// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_ChooseNextWaypoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrolComponent.h"

void UBTT_ChooseNextWaypoint::SetOwner(AActor* InActorOwner)
{
	UE_LOG(LogTemp, Warning, TEXT("C"));
	ActorOwner = InActorOwner;
	AIOwner = Cast<AAIController>(InActorOwner);
}

EBTNodeResult::Type UBTT_ChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIC = OwnerComp.GetAIOwner();
	if (!ensure(AIC)) { return EBTNodeResult::Failed;  }
	APawn* ControlledPawn = AIC->GetPawn();
	if (!ensure(ControlledPawn)) { return EBTNodeResult::Failed; }
	UPatrolComponent* PatrolComponent = ControlledPawn->FindComponentByClass<UPatrolComponent>();
	if (!ensure(PatrolComponent)) { return EBTNodeResult::Failed; }

	auto PatrolRoute = PatrolComponent->GetPatrolRoute();
	if (PatrolRoute.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Patrol Route is empty on %s"), *ControlledPawn->GetName());
	}

	auto BlackBoardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackBoardComp->GetValueAsInt(IndexSelector.SelectedKeyName);
	if (PatrolRoute.Num() > 0)
	{
		int NextIndex = (Index + 1) % PatrolRoute.Num();

		if (PatrolRoute.IsValidIndex(Index))
		{
			BlackBoardComp->SetValueAsObject(WaypointSelector.SelectedKeyName, PatrolRoute[Index]);
		}

		BlackBoardComp->SetValueAsInt(IndexSelector.SelectedKeyName, NextIndex);
	}

	

	return EBTNodeResult::Succeeded;
}