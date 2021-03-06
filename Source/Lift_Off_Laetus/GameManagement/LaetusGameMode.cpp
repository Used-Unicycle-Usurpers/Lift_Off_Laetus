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
#include "GameEnums.h"
#include "LaetusGameInstance.h"
#include "Sound/SoundCue.h"
#include "../PowerUps/CharacterPowerUpEffect.h"

ALaetusGameMode::ALaetusGameMode() {
	// use our custom PlayerController class
	PlayerControllerClass = AInputController::StaticClass();//ACrewController::StaticClass();
	DefaultPawnClass = NULL;

	camera = CreateDefaultSubobject<UCameraComponent>("MainCamera");

	static ConstructorHelpers::FClassFinder<UUserWidget>HUDBlueprintWidgetClass(TEXT("WidgetBlueprintGeneratedClass'/Game/UI/HUD.HUD_C'"));
	HUDWidgetClass = HUDBlueprintWidgetClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget>PauseMenuBlueprintWidgetClass(TEXT("WidgetBlueprint'/Game/UI/PauseMenu.PauseMenu_C'"));
	PauseMenuWidgetClass = PauseMenuBlueprintWidgetClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget>VictoryVideoBlueprintWidgetClass(TEXT("WidgetBlueprint'/Game/Videos/VictoryVideoWidget.VictoryVideoWidget_C'"));
	VictoryVideoWidgetClass = VictoryVideoBlueprintWidgetClass.Class;

	static ConstructorHelpers::FObjectFinder<USoundCue>turn(TEXT("SoundCue'/Game/Audio/AUD_changing_turn01_Cue.AUD_changing_turn01_Cue'"));
	changingTurnSound = turn.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue>error(TEXT("SoundCue'/Game/Audio/AUD_error01_Cue.AUD_error01_Cue'"));
	errorSound = error.Object;
}

/**
 * Called at the very beginning of the game. Sets up the map of AGridSpaces with
 * all the information contained in /Config/grid.txt.
 */
void ALaetusGameMode::BeginPlay() {
	grid = GetWorld()->SpawnActor<AGrid>(FVector(0, 0, 0), FRotator(0, 0, 0));

	hud = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	hud->AddToViewport();

	pauseMenu = CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidgetClass);
	pauseMenu->AddToViewport();

	victoryVideo = CreateWidget<UUserWidget>(GetWorld(), VictoryVideoWidgetClass);

	//Based on the mode selected in the main menu, support keyboard only or keyboard 
	//for player one / gamepad for player two
	ULaetusGameInstance* gameInstance = Cast<ULaetusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	singleInput = gameInstance->singleInput;

	inputController = Cast<AInputController>(UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0));

	//The code below is to test if crew and crewmember are working correctly
	redTeamController = GetWorld()->SpawnActor<ACrewController>();//Cast<ACrewController>(UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0));
	ACrew* redCrew = GetWorld()->SpawnActor<ACrew>(FVector(0, 0, 0), FRotator(0, 0, 0));
	redTeamController->Possess(redCrew);

	blueTeamController = GetWorld()->SpawnActor<ACrewController>();//Cast<ACrewController>(UGameplayStatics::CreatePlayer(GetWorld(), -1, true));
	ACrew* blueCrew = GetWorld()->SpawnActor<ACrew>(FVector(0, 0, 0), FRotator(0, 0, 0));
	blueTeamController->Possess(blueCrew);

	//Initialize Crews and Controllers
	redCrew->SetUp(Team::Red, grid, redTeamController);
	blueCrew->SetUp(Team::Blue, grid, blueTeamController);

	//Initialize the input controller with info on both players of the game.
	if (singleInput) {
		redTeamController->init(redCrew, inputController);
		blueTeamController->init(blueCrew, inputController);
		inputController->init(redTeamController, blueTeamController);
	}else {
		inputController2 = Cast<AInputController>(UGameplayStatics::CreatePlayer(GetWorld(), -1, true));

		redTeamController->init(redCrew, inputController);
		blueTeamController->init(blueCrew, inputController2);
		inputController->init(redTeamController, nullptr);
		inputController2->init(nullptr, blueTeamController);
	}

	// add to crews array
	crews.Add(redCrew);
	crews.Add(blueCrew);

	// Assign core fragment receivers to respective teams
	grid->assignCoreFragmentReceivers(redCrew, blueCrew);

	callHUDSetInputControllers(inputController, inputController2, singleInput);
	callHUDSetCrews(redCrew, blueCrew);

	//Begin first turn
	currentCrew = -1;
	ChangeTurn();
}

//Begin new turn
void ALaetusGameMode::ChangeTurn() {
	UGameplayStatics::PlaySound2D(GetWorld(), changingTurnSound);
	grid->clearGridOverlay();

	//inform player we are switching turns 
	if (firstChangeTurn) {
		firstChangeTurn = false;
	}else {
		callHUDMessage(true, 10);
	}
	callHUDToggleThrowGrenadeInstruction(ESlateVisibility::Hidden);

	//Swap to the other team
	currentCrew += 1;
	if (currentCrew > crewCount - 1){
		currentCrew = 0;
	}	

	//set actionbar to 10 and update hud 
	actionbar = 10;
	callHUDUpdateAB(actionbar);

	ACrew* newCrew = crews[currentCrew];
	newCrew->setSelectedCrewMember(0);
	if (currentCrew == 0) {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("Red Team's Turn"));
		inputController->changeTurn(currentCrew);
		inputController->resetInputMode();
		if (!singleInput) {
			inputController->enable();
			inputController2->disable();
			inputController2->changeTurn(currentCrew);
			inputController2->resetInputMode();
		}
	}else {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, TEXT("Blue Team's Turn"));
		inputController->changeTurn(currentCrew);
		inputController->resetInputMode();
		if (!singleInput) {
			inputController->disable();
			inputController2->enable();
			inputController2->changeTurn(currentCrew);
			inputController2->resetInputMode();
		}
	}

	callHUDSetPlayer(-1);

	FsetTeamParams params1;
	params1.teamIndex = currentCrew;
	UFunction* setTeamFunction = hud->FindFunction(FName("setTeam"));
	hud->ProcessEvent(setTeamFunction, &params1);

	callHUDSetPlayer(0);

	//hide change turn message
	if (!firstChangeTurn) {
		message = 10;
		visible = false;
		GetWorldTimerManager().SetTimer(ChangeTurnTimerHandle, this, &ALaetusGameMode::callHUDTimer, 1.0f, false);
	}


	UActorComponent* powerUp;

	//TArray<class ACrewMember*> redTeam = crews[0]->crewMembers;
	//TArray<class ACrewMember*> blueTeam = crews[1]->crewMembers;

	for (int i = 0; i < 3; i++) {
		powerUp = crews[0]->crewMembers[i]->GetComponentByClass(UBasePowerUpEffectComponent::StaticClass());
		if (IsValid(powerUp)) {
			UE_LOG(LogTemp, Warning, TEXT("Bonk 0"));
			Cast<UBasePowerUpEffectComponent>(powerUp)->DecrementLife(this, 0, i);
		}

		powerUp = crews[1]->crewMembers[i]->GetComponentByClass(UBasePowerUpEffectComponent::StaticClass());
		if (IsValid(powerUp)) {
			UE_LOG(LogTemp, Warning, TEXT("Bonk 1"));
			Cast<UBasePowerUpEffectComponent>(powerUp)->DecrementLife(this, 1, i);
		}
	}

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
	inputController->disable();
	if (inputController2) {
		inputController2->disable();
	}

	if (winner == 0) {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, TEXT("Red Team Won!"));
		callVictoryVideoPlayWinningVideo(0);
	} else if (winner == 1) {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, TEXT("Blue Team Won!"));
		callVictoryVideoPlayWinningVideo(1);
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("Error: Unrecognized team won"));
	}
}

void ALaetusGameMode::ClearTurnActionStack() {
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

void ALaetusGameMode::callHUDUpdateAB(int32 status) {
	FsetABParams params;
	params.status = status;
	UFunction * updateActionBar = hud->FindFunction(FName("updateActionBar"));
	hud->ProcessEvent(updateActionBar, &params);
}

void ALaetusGameMode::callHUDMessage(bool vis, int32 mess) {
	FsetHUDMessageParams params;
	params.visible = vis;
	params.message = mess;
	UFunction * displayMessage = hud->FindFunction(FName("displayMessage"));
	hud->ProcessEvent(displayMessage, &params);
}

int32 ALaetusGameMode::getABStatus() {
	return actionbar;
}

void ALaetusGameMode::callHUDTimer() {
	callHUDMessage(visible, message);
}

// We check if we have enough action points to complete a move 
// if so we update actionbar
bool ALaetusGameMode::checkLegalMove(int32 actionPrice) {
	int32 pointsLeft = actionbar - actionPrice;
	if (pointsLeft >= 0) { //update actionbar, no need to change turn
		actionbar -= actionPrice;
		callHUDUpdateAB(actionbar);

		//inform player of how many action point spent 
		callHUDMessage(true, actionPrice);

		//hide message after a bit
		message = actionPrice;
		visible = false;
		GetWorldTimerManager().SetTimer(ActionPriceTimerHandle, this, &ALaetusGameMode::callHUDTimer, 1.5f, false);
	}else { //not enough action points
		//show invalid move message and play error sound
		callHUDMessage(true, 0);
		UGameplayStatics::PlaySound2D(GetWorld(), errorSound);
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("Called display message"));

		//display invalid move message for a bit
		visible = false;
		message = 0;
		if (!GetWorld()->GetTimerManager().IsTimerActive(NoPointsTimerHandle)) {
			GetWorldTimerManager().SetTimer(NoPointsTimerHandle, this, &ALaetusGameMode::callHUDTimer, 1.5f, false);
		}
		return false;
	}

	return true;
}

void ALaetusGameMode::callHUDSetInputControllers(AInputController* c1, AInputController* c2, bool singleInputOnly) {
	FsetInputControllers params;
	params.controller1 = c1;
	params.controller2 = c2;
	params.twoInputs = singleInputOnly;
	UFunction* setInputControllesFunction = hud->FindFunction(FName("setInputControllers"));
	hud->ProcessEvent(setInputControllesFunction, &params);
}

void ALaetusGameMode::callHUDSetCrews(class ACrew* c1, class ACrew* c2) {
	FsetCrews params;
	params.redCrew = c1;
	params.blueCrew = c2;
	UFunction* setCrewsFunction = hud->FindFunction(FName("setCrews"));
	hud->ProcessEvent(setCrewsFunction, &params);
}

void ALaetusGameMode::callHUDToggleThrowGrenadeInstruction(ESlateVisibility newVisibility) {
	FsetThrowGrenadeVisibility params;
	params.visibility = newVisibility;
	UFunction* setCrewsFunction = hud->FindFunction(FName("setThrowGrenadeVisibility"));
	hud->ProcessEvent(setCrewsFunction, &params);
}

void ALaetusGameMode::callHUDSetEffectOverlay(int teamIndex, int playerIndex, UTexture2D* overlayTexture) {
	FSetEffectOverlayParams params;
	params.teamIndex = teamIndex;
	params.playerIndex = playerIndex;
	params.overlayTexture = overlayTexture;
	UFunction* setOverlayFunction = hud->FindFunction(FName("SetEffectOverlay"));
	hud->ProcessEvent(setOverlayFunction, &params);
}

void ALaetusGameMode::callPauseMenuToggleVisibility() {
	UFunction* setCrewsFunction = pauseMenu->FindFunction(FName("toggleVisibility"));
	pauseMenu->ProcessEvent(setCrewsFunction, nullptr);
}

void ALaetusGameMode::callVictoryVideoPlayWinningVideo(int team) {
	victoryVideo->AddToViewport();
	FplayWinningVideo params;
	params.winningTeam = team;
	UFunction* playWinningVideoFunction = victoryVideo->FindFunction(FName("playWinningVideo"));
	victoryVideo->ProcessEvent(playWinningVideoFunction, &params);
}