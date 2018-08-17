// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ShooterGameGameMode.h"
#include "HUD/ShooterGameHUD.h"
#include "PlayerCharacter/FirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShooterGameGameMode::AShooterGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Characters/PlayerCharacter/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AShooterGameHUD::StaticClass();
}
