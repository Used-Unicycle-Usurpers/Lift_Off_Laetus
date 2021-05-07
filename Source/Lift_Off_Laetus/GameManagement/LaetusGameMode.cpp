// Fill out your copyright notice in the Description page of Project Settings.

#include "LaetusGameMode.h"
#include "../Controllers/CrewController.h"
#include "../Controllers/InputController.h"
#include "Lift_Off_Laetus/Characters/Crew.h"
#include "GridSpace.h"
#include "CoreFragmentReceiver.h"
#include "Grid.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Engine/Engine.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

ALaetusGameMode::ALaetusGameMode() {
	// use our custom PlayerController class
	PlayerControllerClass = AInputController::StaticClass();//ACrewController::StaticClass();
	DefaultPawnClass = NULL;

	camera = CreateDefaultSubobject<UCameraComponent>("MainCamera");

	static ConstructorHelpers::FClassFinder<UUserWidget>HUDBlueprintWidgetClass(TEXT("WidgetBlueprintGeneratedClass'/Game/UI/HUD.HUD_C'"));
	HUDWidgetClass = HUDBlueprintWidgetClass.Class;

	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn>PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/
}

/**
 * Called at the very beginning of the game. Sets up the map of AGridSpaces with
 * all the information contained in /Config/grid.txt.
 */
void ALaetusGameMode::BeginPlay() {
	grid = GetWorld()->SpawnActor<AGrid>(FVector(0, 0, 0), FRotator(0, 0, 0));

	hud = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	hud->AddToViewport();

	inputController = Cast<AInputController>(UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0));

	//The code below is to test if crew and crewmember are working correctly
	redTeamController = GetWorld()->SpawnActor<ACrewController>();//Cast<ACrewController>(UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0));
	ACrew* redCrew = GetWorld()->SpawnActor<ACrew>(FVector(0, 0, 0), FRotator(0, 0, 0)); 
	redTeamController->Possess(redCrew);
	
	blueTeamController = GetWorld()->SpawnActor<ACrewController>();//Cast<ACrewController>(UGameplayStatics::CreatePlayer(GetWorld(), -1, true));
	ACrew* blueCrew = GetWorld()->SpawnActor<ACrew>(FVector(0, 0, 0), FRotator(0, 0, 0));
	blueTeamController->Possess(blueCrew);
	
	//Initialize Crews and Controllers
	redCrew->SetUp(0, grid, redTeamController);
	redTeamController->init(redCrew, inputController);

	blueCrew->SetUp(1, grid, blueTeamController);
	blueTeamController->init(blueCrew, inputController);

	//Initialize the input controller with info on both players of the game.
	inputController->init(redTeamController, blueTeamController);

	// add to crews array
	crews.Add(redCrew);
	crews.Add(blueCrew);

	// Assign core fragment receivers to respective teams
	grid->assignCoreFragmentReceivers(redCrew, blueCrew);

	//Begin first turn
	currentCrew = -1;
	ChangeTurn();
}

//Begin new turn
void ALaetusGameMode::ChangeTurn() {
	//Get location of first crew member in new team
	currentCrew += 1;
	if (currentCrew > crewCount - 1){
		currentCrew = 0;
	}
	inputController->changeTurn(currentCrew);

	ACrew* newCrew = crews[currentCrew];
	newCrew->setSelectedCrewMember(0);
	if (currentCrew == 0) {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("Red Team's Turn"));
		//redTeamController->enable();
		//blueTeamController->disable();
	}else {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, TEXT("Blue Team's Turn"));
		//blueTeamController->enable();
		//redTeamController->disable();
	}

	callHUDSetPlayer(-1);

	FsetTeamParams params1;
	params1.teamIndex = currentCrew;
	UFunction* setTeamFunction = hud->FindFunction(FName("setTeam"));
	hud->ProcessEvent(setTeamFunction, &params1);

	callHUDSetPlayer(0);
}

void ALaetusGameMode::EvaluateWin()
{
	// Check if the current crew has same or more cores than 'coresToWin'
	// Return -1 if neither, 0 if team 0, or 1 if team 1

	// TODO: Possibly check other conditions? e.g. no crew members remaining, no cores available, etc.

	int winner = -1;

	bool redWon = (crews[0]->getCoreCount() >= coresToWin);
	bool blueWon = (crews[0]->getCoreCount() >= coresToWin);

	// Check which team won
	if (blueWon && redWon) {
		winner = currentCrew; // No room for ties - just pick the current team?
	} else if (redWon) {
		winner = 0;
	} else if (blueWon) {
		winner = 1;
	}

	// End the game if a team has won
	if (winner != -1) {
		OnGameEnd(winner);
	} else {
		ChangeTurn();
	}
}

void ALaetusGameMode::OnGameEnd(int32 winner) {

	// TODO: End turn, lock inputs

	//redTeamController->disable();
	//blueTeamController->disable();

	if (winner == 0) {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, TEXT("Red Team Won!"));
	} else if (winner == 1) {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, TEXT("Blue Team Won!"));
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("Error: Unrecognized team won"));
	}
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

AGrid* ALaetusGameMode::getGameGrid() {
	return grid;
}

void ALaetusGameMode::callHUDSetPlayer(int newPlayerIndex) {
	FsetPlayerParams params;
	params.playerIndex = newPlayerIndex;
	UFunction* setPlayerFunction = hud->FindFunction(FName("setPlayer"));
	hud->ProcessEvent(setPlayerFunction, &params);
}