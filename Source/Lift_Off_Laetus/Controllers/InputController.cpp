// Fill out your copyright notice in the Description page of Project Settings.


#include "InputController.h"
#include "../Characters/Crew.h"
#include "../Characters/CrewMember.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "../GameManagement/LaetusGameMode.h"
#include "../GameManagement/GridSpace.h"
#include "../GameManagement/Grid.h"
#include "../Controllers/CrewController.h"

AInputController::AInputController() {

}

void AInputController::BeginPlay() {
	Super::BeginPlay();
	gameMode = Cast<ALaetusGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AInputController::SetupInputComponent() {
	Super::SetupInputComponent();

	//Inputs for swithcing between crew members and turns
	InputComponent->BindAction("ToggleCrewMember", IE_Pressed, this, &AInputController::toggleCrewMember);
	InputComponent->BindAction("EndTurn", IE_Pressed, this, &AInputController::endTurn);

	//These are the basic 4 controls for moving and attacking. They will be interpreteed differently 
	//depending on what turn state the player is currently in.
	InputComponent->BindAction("Up", IE_Pressed, this, &AInputController::handleUp);
	InputComponent->BindAction("Left", IE_Pressed, this, &AInputController::handleLeft);
	InputComponent->BindAction("Right", IE_Pressed, this, &AInputController::handleRight);
	InputComponent->BindAction("Down", IE_Pressed, this, &AInputController::handleDown);

	InputComponent->BindAction("Confirm", IE_Pressed, this, &AInputController::handleConfirm);

	//For debugging, these keybinds allow you to manually set the state, rather then selecting move, 
	//attack, or collect on the UI
	InputComponent->BindAction("SetToMovement", IE_Pressed, this, &AInputController::setStateToMovement);
	InputComponent->BindAction("SetToRifleAttack", IE_Pressed, this, &AInputController::setStateToRifleAttack);
	InputComponent->BindAction("SetToGrenadeAttack", IE_Pressed, this, &AInputController::setStateToGrenadeAttack);
	InputComponent->BindAction("SetToHarvest", IE_Pressed, this, &AInputController::setStateToHarvest);

	PlayerCameraManagerClass = PlayerCameraManager->GetClass();
}

void AInputController::init(ACrewController* redController, ACrewController* blueController) {
	cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	currentTurnState = FTurnState::Idle;
	redTeamController = redController;
	blueTeamController = blueController;
	if (redController && blueController) {
		twoPlayers = true;
	}else {
		twoPlayers = false;

		//Set current to the only controller that was provided
		currentTeamController = (redTeamController != nullptr) ? redTeamController : blueTeamController;
		controlledCrew = currentTeamController->getControlledCrew();
	}
}

void AInputController::changeTurn(int newTeam) {
	if (twoPlayers) {
		if (newTeam == 0) {
			currentTeamController = redTeamController;
			controlledCrew = currentTeamController->getControlledCrew();
		}else {
			currentTeamController = blueTeamController;
			controlledCrew = currentTeamController->getControlledCrew();
		}
	}
}

/**
 * Enable input on this controller.
 */
void AInputController::enable() {
	EnableInput(this);
}

/**
 * Disable input on this controller.
 */
void AInputController::disable() {
	DisableInput(this);
}

/**
 * Tell the game mode to end this players turn and switch control over to
 * the other player.
 */
void AInputController::endTurn() {
	if (currentlySelectedTile) {
		currentlySelectedTile->SetToRegularMaterial();
	}

	gameMode->EvaluateWin();  // Check for winner + change turn if no one won (order subject to change)
}

/**
 * Moves the camera to the current ACrewMember.
 */
void AInputController::moveCameraToCrewMember() {
	if (controlledCrew) {
		ACrewMember* current = controlledCrew->getCurrentCrewMember();
		FViewTargetTransitionParams p;
		p.BlendFunction = EViewTargetBlendFunction::VTBlend_Linear;
		p.BlendTime = 1.f;
		cameraManager->SetViewTarget(current, p);
	}
}

/**
 * Toggle the currently selected crew member and focus the camera
 * on them
 */
void AInputController::toggleCrewMember() {
	//Remove the yellow highlight on the HUD for the crew member that 
	//is no longer selected.
	gameMode->callHUDSetPlayer(-1);

	//Toggle and highlight character portrait on the HUDs
	controlledCrew->toggleSelectedCrewMember();
	gameMode->callHUDSetPlayer(controlledCrew->getSelectedCrewMemberIndex());

	//Now move the camera to focus on this crew member.
	moveCameraToCrewMember();
}

/**
 * Set the state of the turn this player is in to the given state.
 *
 * @param newState the turn state this player is now entering.
 */
void AInputController::setTurnState(enum FTurnState newState) {
	currentTurnState = newState;
}

void AInputController::setStateToMovement() {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("NOW IN MOVEMENT MODE"));
	gameMode->getGameGrid()->clearGridOverlay();
	if (controlledCrew) {
		if (currentlySelectedTile) {
			currentlySelectedTile->SetToRegularMaterial();
		}
		moveCameraSmoothly(controlledCrew->getCurrentCrewMember());
		setTurnState(Movement);
	}
}

void AInputController::setStateToRifleAttack() {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("NOW IN RIFLE ATTACK MODE"));
	gameMode->getGameGrid()->clearGridOverlay();
	if (controlledCrew) {
		if (currentlySelectedTile) {
			currentlySelectedTile->SetToRegularMaterial();
		}
		moveCameraSmoothly(controlledCrew->getCurrentCrewMember());
		const FVector2D origin = controlledCrew->getCurrentCrewMember()->getGridSpace()->getGridLocation();
		gameMode->getGameGrid()->colorGridDirectionsInRange(origin, 5);
		setTurnState(RifleAttack);
	}
}

void AInputController::setStateToGrenadeAttack() {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("NOW IN GRENADE ATTACK MODE"));
	gameMode->getGameGrid()->clearGridOverlay();

	//Start by focusing on current tile. WASD will now move highlighted so player can select 
	//where to throw the grenade.
	if (controlledCrew) {
		currentlySelectedTile = controlledCrew->getCurrentCrewMember()->getGridSpace();
		moveCameraSmoothly(currentlySelectedTile);
		setTurnState(GrenadeAttack);
		gameMode->getGameGrid()->colorGridInRange(currentlySelectedTile->getGridLocation(), 2);
	}
}

void AInputController::setStateToHarvest() {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("NOW IN HARVEST MODE"));
	gameMode->getGameGrid()->clearGridOverlay();
	if (controlledCrew) {
		if (currentlySelectedTile) {
			currentlySelectedTile->SetToRegularMaterial();
		}
		moveCameraSmoothly(controlledCrew->getCurrentCrewMember());
		setTurnState(Harvest);
	}
}

void AInputController::setStateToIdle() {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("NOW IN IDLE MODE"));
	gameMode->getGameGrid()->clearGridOverlay();
	if (controlledCrew) {
		if (currentlySelectedTile) {
			currentlySelectedTile->SetToRegularMaterial();
		}
		moveCameraSmoothly(controlledCrew->getCurrentCrewMember());
		setTurnState(Idle);
	}
}

/**
 * Handle the "Up" key based on the current turn state this player is in.
 */
void AInputController::handleUp() {
	ACrewMember* current;
	bool canMove;
	FVector2D direction;
	switch (currentTurnState) {
	case Idle:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing up in Idle state"));
		break;
	case CameraMovement:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing up in Camera state"));
		break;
	case Movement:
		//Price for movement is 1, if we are pushing core its 2
		//see if we are pushing core
		price = 1;
		direction = DirectionToUnitVector(Direction::Up);
		if (controlledCrew->pushingCore(direction)) { price = 2; }

		current = controlledCrew->getCurrentCrewMember();
		canMove = gameMode->getGameGrid()->canMove(current->getGridSpace(), direction);
		if (canMove && gameMode->checkLegalMove(price)) {
			controlledCrew->moveSelectedCrewMember(direction);
		}
		break;
	case RifleAttack:
		//Price for rifle attack is 3 
		price = 3;
		if (gameMode->checkLegalMove(price)) {
				currentTeamController->shoot(FVector2D(-1, 0));
		}
		break;
	case GrenadeAttack:
		moveCameraToTile(Direction::Up);
		break;
	case Harvest:
		// For now I was thinking we set the price of harvest to 2
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing up in Harvest state"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing up in default state"));
	}
}

/**
 * Handle the "Left" input based on the current turn state this player is in.
 */
void AInputController::handleLeft() {
	ACrewMember* current;
	bool canMove;
	FVector2D direction;
	switch (currentTurnState) {
	case Idle:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing left in Idle state"));
		break;
	case CameraMovement:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing left in Camera state"));
		break;
	case Movement:
		//Price for movement is 1, if we are pushing core its 2
		//see if we are pushing core
		price = 1;
		direction = DirectionToUnitVector(Direction::Left);
		if (controlledCrew->pushingCore(direction)) { price = 2; }

		current = controlledCrew->getCurrentCrewMember();
		canMove = gameMode->getGameGrid()->canMove(current->getGridSpace(), direction);
		if (canMove && gameMode->checkLegalMove(price)) {
			controlledCrew->moveSelectedCrewMember(direction);
		}
		break;
	case RifleAttack:
		//Price for rifle attack is 3 
		price = 3;
		if (gameMode->checkLegalMove(price)) {
			currentTeamController->shoot(FVector2D(0, -1));
		}
		break;
	case GrenadeAttack:
		moveCameraToTile(Direction::Left);
		break;
	case Harvest:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing left in Harvest state"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing left in default state"));
	}
}

/**
 * Handle the "Right" input based on the current turn state this player is in.
 */
void AInputController::handleRight() {
	ACrewMember* current;
	bool canMove;
	FVector2D direction;
	switch (currentTurnState) {
	case Idle:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing right in Idle state"));
		break;
	case CameraMovement:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing right in Camera state"));
		break;
	case Movement:
		//Price for movement is 1, if we are pushing core its 2
		//see if we are pushing core
		price = 1;
		direction = DirectionToUnitVector(Direction::Right);
		if (controlledCrew->pushingCore(direction)) { price = 2; }
		
		current = controlledCrew->getCurrentCrewMember();
		canMove = gameMode->getGameGrid()->canMove(current->getGridSpace(), direction);
		if (canMove && gameMode->checkLegalMove(price)) {
			controlledCrew->moveSelectedCrewMember(direction);
		}
		break;
	case RifleAttack:
		//Price for rifle attack is 3 
		price = 3;
		
		if (gameMode->checkLegalMove(price)) {
			currentTeamController->shoot(FVector2D(0, 1));
		}
		break;
	case GrenadeAttack:
		moveCameraToTile(Direction::Right);
		break;
	case Harvest:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing right in Harvest state"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing right in default state"));
	}
}

/**
 * Handle the "Down" input based on the current turn state this player is in.
 */
void AInputController::handleDown() {
	ACrewMember* current;
	bool canMove;
	FVector2D direction;
	switch (currentTurnState) {
	case Idle:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing down in Idle state"));
		break;
	case CameraMovement:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing down in Camera state"));
		break;
	case Movement:
		//Price for movement is 1, if we are pushing core its 2
		//see if we are pushing core
		price = 1;
		direction = DirectionToUnitVector(Direction::Down);
		if (controlledCrew->pushingCore(direction)) { price = 2; }
		
		current = controlledCrew->getCurrentCrewMember();
		canMove = gameMode->getGameGrid()->canMove(current->getGridSpace(), direction);
		if (canMove && gameMode->checkLegalMove(price)) {
			controlledCrew->moveSelectedCrewMember(direction);
		}
		break;
	case RifleAttack:
		//Price for rifle attack is 3 
		price = 3;
		if (gameMode->checkLegalMove(price)) {
			currentTeamController->shoot(FVector2D(1, 0));
		}
		break;
	case GrenadeAttack:
		moveCameraToTile(Direction::Down);
		break;
	case Harvest:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing down in Harvest state"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing down in default state"));
	}
}

/**
 * Handle the "Confim" input based on the current turn state this player is in.
 */
void AInputController::handleConfirm() {
	switch (currentTurnState) {
	case Idle:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing Confirm in Idle state"));
		break;
	case CameraMovement:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing Confirm in Camera state"));
		break;
	case Movement:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing Confirm in Movement state"));
		break;
	case RifleAttack:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing Confirm in RifleAttack state"));
		break;
	case GrenadeAttack:
		//Price for grenade attack is 5
		price = 5;
		if (gameMode->checkLegalMove(price)) {
			currentTeamController->launch(currentlySelectedTile->getGridLocation());
		}
		break;
	case Harvest:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing Confirm in Harvest state"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing Confirm in default state"));
	}
}

/**
 * Move camera to the next AGridSpace in the specified direction.
 *
 * @param direction a Direction enum specifying the direction of
 *     the next tile to move the camera to.
 */
void AInputController::moveCameraToTile(Direction direction) {
	AGrid* grid = gameMode->getGameGrid();

	FVector2D directionVector = FVector2D(0, 0);
	switch (direction) {
	case Up:
		directionVector = FVector2D(-1, 0);
		break;
	case Left:
		directionVector = FVector2D(0, -1);
		break;
	case Right:
		directionVector = FVector2D(0, 1);
		break;
	case Down:
		directionVector = FVector2D(1, 0);
		break;
	default:
		return;
	}

	FVector2D currentLocation = currentlySelectedTile->getGridLocation();
	FVector2D newLocation = currentLocation + directionVector;
	FVector2D origin = controlledCrew->getCurrentCrewMember()->getGridSpace()->getGridLocation();
	if (grid->areTilesWithinRange(origin, newLocation, 2)) {

		AGridSpace* newSpace = grid->getTile(newLocation);
		if (newSpace) {
			currentlySelectedTile->RestoreOverlayColor();
			currentlySelectedTile = newSpace;
			currentlySelectedTile->SetOverlayToGreen(true);

			moveCameraSmoothly(currentlySelectedTile);
		}
	}
}

/**
 * Move the camera smoothly from its current location to the target actor.
 *
 * @param target the actor to smoothly move the camera to.
 */
void AInputController::moveCameraSmoothly(AActor* target) {
	FViewTargetTransitionParams p;
	p.BlendFunction = EViewTargetBlendFunction::VTBlend_Linear;
	p.BlendTime = 0.1f;
	cameraManager->SetViewTarget(target, p);
}