// Copyright Epic Games, Inc. All Rights Reserved.

#include "Lift_Off_LaetusGameMode.h"
#include "Lift_Off_LaetusPlayerController.h"
#include "Lift_Off_LaetusCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALift_Off_LaetusGameMode::ALift_Off_LaetusGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ALift_Off_LaetusPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}