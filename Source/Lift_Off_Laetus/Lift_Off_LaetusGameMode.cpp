// Copyright Epic Games, Inc. All Rights Reserved.

#include "Lift_Off_LaetusGameMode.h"
#include "Lift_Off_LaetusPlayerController.h"
#include "Lift_Off_LaetusCharacter.h"
#include "Controllers/CrewController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "Misc/FileHelper.h"
#include "PowerUps/SlimeTree.h"
#include "PowerUps/Rock.h"
#include "PowerUps/Shrub.h"
#include "UObject/ConstructorHelpers.h"

ALift_Off_LaetusGameMode::ALift_Off_LaetusGameMode(){
	// use our custom PlayerController class
	PlayerControllerClass = ACrewController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr){
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

/**
 * Called at the very beginning of the game. Sets up the map of AGridSpaces with
 * all the information contained in /Config/grid.txt.
 */
void ALift_Off_LaetusGameMode::BeginPlay() {
	grid = GetWorld()->SpawnActor<AGrid>(FVector(0, 0, 0), FRotator(0, 0, 0));
}
