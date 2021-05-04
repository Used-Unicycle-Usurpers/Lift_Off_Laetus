// Fill out your copyright notice in the Description page of Project Settings.

#include "LaetusGameMode.h"
#include "../Controllers/CrewController.h"
#include "Lift_Off_Laetus/Characters/Crew.h"
#include "GridSpace.h"
#include "Grid.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Engine/Engine.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TheActionBar.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"


ALaetusGameMode::ALaetusGameMode() {
	// use our custom PlayerController class
	PlayerControllerClass = ACrewController::StaticClass();
	DefaultPawnClass = NULL;

	camera = CreateDefaultSubobject<UCameraComponent>("MainCamera");

	static ConstructorHelpers::FClassFinder<UUserWidget>HUDBlueprintWidgetClass(TEXT("WidgetBlueprintGeneratedClass'/Game/UI/HUD.HUD_C'"));
	HUDWidgetClass = HUDBlueprintWidgetClass.Class;

	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn>PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/

	//set action bar 
	ActionBarComp = CreateDefaultSubobject<UWidgetComponent>("ActionBar");
	//ActionBar = CreateDefaultSubobject<UProgressBar>("ActionBar");
}


/**
 * Called at the very beginning of the game. Sets up the map of AGridSpaces with
 * all the information contained in /Config/grid.txt.
 */
void ALaetusGameMode::BeginPlay() {
	grid = GetWorld()->SpawnActor<AGrid>(FVector(0, 0, 0), FRotator(0, 0, 0));

	hud = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	hud->AddToViewport();

	//The code below is to test if crew and crewmember are working correctly
	redTeamController = Cast<ACrewController>(UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0));
	ACrew* redCrew = GetWorld()->SpawnActor<ACrew>(FVector(0, 0, 0), FRotator(0, 0, 0));
	redTeamController->Possess(redCrew);

	blueTeamController = Cast<ACrewController>(UGameplayStatics::CreatePlayer(GetWorld(), -1, true));
	ACrew* blueCrew = GetWorld()->SpawnActor<ACrew>(FVector(0, 0, 0), FRotator(0, 0, 0));
	blueTeamController->Possess(blueCrew);

	//set teams
	redCrew->SetUp(0, grid);
	blueCrew->SetUp(1, grid);

	// add to crews array
	crews.Add(redCrew);
	crews.Add(blueCrew);

	//Begin first turn
	currentCrew = -1;
	ChangeTurn();


	//ActionBar = CreateWidget<UProgressBar>(GetWorld(), HUDWidgetClass);
	actionBar = Cast<UTheActionBar>(ActionBarComp->GetUserWidgetObject());

	actionBar->UpdateActionBar(100.0);
	//actionBar = CreateDefaultSubobject<UWidgetComponent>(this, TEXT("ActionBar"));
}

//Begin new turn
void ALaetusGameMode::ChangeTurn() {
	//CHANGE CAMERA FOCUS TO NEW CREW
	//Get location of first crew member in new team
	currentCrew += 1;
	if (currentCrew > crewCount - 1){
		currentCrew = 0;
	}

	ACrew* newCrew = crews[currentCrew];
	newCrew->setSelectedCrewMember(0);
	if (currentCrew == 0) {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("Red Team's Turn"));
		redTeamController->enable();
		blueTeamController->disable();
	}else {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, TEXT("Blue Team's Turn"));
		blueTeamController->enable();
		redTeamController->disable();
	}

	callHUDSetPlayer(-1);

	FsetTeamParams params1;
	params1.teamIndex = currentCrew;
	UFunction* setTeamFunction = hud->FindFunction(FName("setTeam"));
	hud->ProcessEvent(setTeamFunction, &params1);

	callHUDSetPlayer(0);

	// Update action bar 
	float actionBarStat = newCrew->GetActionBarStatus();
	//actionBar->UpdateActionBar(50);
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

AGrid* ALaetusGameMode::getGameGrid() {
	return grid;
}

void ALaetusGameMode::callHUDSetPlayer(int newPlayerIndex) {
	FsetPlayerParams params;
	params.playerIndex = newPlayerIndex;
	UFunction* setPlayerFunction = hud->FindFunction(FName("setPlayer"));
	hud->ProcessEvent(setPlayerFunction, &params);
}