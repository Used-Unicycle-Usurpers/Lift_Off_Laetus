// Fill out your copyright notice in the Description page of Project Settings.

#include "LaetusGameMode.h"
#include "../Controllers/CrewController.h"
#include "Lift_Off_Laetus/Characters/Crew.h"
#include "GridSpace.h"
#include "Grid.h"
#include "Kismet/GameplayStatics.h"

ALaetusGameMode::ALaetusGameMode() {
	// use our custom PlayerController class
	PlayerControllerClass = ACrewController::StaticClass();
	DefaultPawnClass = NULL;

	// set default pawn class to our Blueprinted character
	/*
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	*/
}

/**
 * Called at the very beginning of the game. Sets up the map of AGridSpaces with
 * all the information contained in /Config/grid.txt.
 */
void ALaetusGameMode::BeginPlay() {
	
	// Make grid
	grid = GetWorld()->SpawnActor<AGrid>(FVector(0, 0, 0), FRotator(0, 0, 0));

	//The code below is to test if crew and crewmember are working correctly
	redTeamController = Cast<ACrewController>(UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0));
	ACrew* redTeam = GetWorld()->SpawnActor<ACrew>(FVector(0, 0, 0), FRotator(0, 0, 0)); 
	redTeamController->Possess(redTeam);
	
	blueTeamController = Cast<ACrewController>(UGameplayStatics::CreatePlayer(GetWorld(), -1, true));
	ACrew* blueTeam = GetWorld()->SpawnActor<ACrew>(FVector(0, 0, 0), FRotator(0, 0, 0));
	blueTeamController->Possess(blueTeam);
	
	//set teams
	redTeam->SetUp(0, grid);
	blueTeam->SetUp(1, grid);

	// add to crews array
	crews.Add(redTeam);
	crews.Add(blueTeam);
}


void ALaetusGameMode::ChangeTurn()
{
	// Cycle through crews
	currentCrew += 1;

	if (currentCrew > crewCount - 1)
	{
		currentCrew = 0;
	}

	// TODO - Do something with 'crews[currentCrew]', i.e. indicate it's the next crew's turn
}


int ALaetusGameMode::EvaluateWin()
{
	// Check if the current crew has same or more cores than 'coresToWin'
	// Return 1 if true, 0 if false

	// Possibly check other conditions? e.g. no crew members remaining, no cores available, etc.

	return 0;
}

void ALaetusGameMode::ClearTurnActionStack()
{
	// Remove all entries in turn action stack
}

APawn* ALaetusGameMode::SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot) {
	return NULL;
};

UClass* ALaetusGameMode::GetDefaultPawnClassForController(AController* InController) {
	return NULL;
}
