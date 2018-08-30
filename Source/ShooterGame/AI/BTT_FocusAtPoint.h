// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FocusAtPoint.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UBTT_FocusAtPoint : public UBTTaskNode
{
	GENERATED_BODY()
	

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void SetOwner(AActor* InActorOwner) override;

public:
	/** Cached AIController owner of BehaviorTreeComponent. */
	UPROPERTY(Transient)
	class AAIController* AIOwner;

	/** Cached actor owner of BehaviorTreeComponent. */
	UPROPERTY(Transient)
	class AActor* ActorOwner;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector FocalPointSelector;

};