// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "SG_PatrollingGuard.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API ASG_PatrollingGuard : public ATP_ThirdPersonCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, Category = "AI")
	TArray<AActor*> PatrolRoute;
	
};
