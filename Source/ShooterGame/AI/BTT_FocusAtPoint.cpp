// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_FocusAtPoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrolComponent.h"



void UBTT_FocusAtPoint::SetOwner(AActor* InActorOwner)
{
	UE_LOG(LogTemp, Warning, TEXT("C"));
	ActorOwner = InActorOwner;
	AIOwner = Cast<AAIController>(InActorOwner);
}

EBTNodeResult::Type UBTT_FocusAtPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIC = OwnerComp.GetAIOwner();
	if (!ensure(AIC)) { return EBTNodeResult::Failed; }
	APawn* ControlledPawn = AIC->GetPawn();
	if (!ensure(ControlledPawn)) { return EBTNodeResult::Failed; }

	auto BlackBoardComp = OwnerComp.GetBlackboardComponent();
	auto FocalActor = Cast<AActor>(BlackBoardComp->GetValueAsObject(FocalPointSelector.SelectedKeyName));
	AIC->SetFocus(FocalActor);

	return EBTNodeResult::Succeeded;
}
