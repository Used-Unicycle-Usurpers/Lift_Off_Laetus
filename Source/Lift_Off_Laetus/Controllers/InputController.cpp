// Fill out your copyright notice in the Description page of Project Settings.


#include "InputController.h"
#include "../Characters/Crew.h"
#include "../Characters/CrewMember.h"
#include "../Characters/CoreFragment.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "../GameManagement/LaetusGameMode.h"
#include "../GameManagement/GridSpace.h"
#include "../GameManagement/Grid.h"
#include "../Controllers/CrewController.h"
#include "../PowerUps/TileStickyEffect.h"
#include "../PowerUps/CharacterStickyEffect.h"

AInputController::AInputController() {

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
	InputComponent->BindAction("Pause", IE_Pressed, this, &AInputController::handlePause);

	//For debugging, these keybinds allow you to manually set the state, rather then selecting move, 
	//attack, or collect on the UI
	InputComponent->BindAction("SetToMovement", IE_Pressed, this, &AInputController::setStateToMovement);
	InputComponent->BindAction("SetToRifleAttack", IE_Pressed, this, &AInputController::setStateToRifleAttack);
	InputComponent->BindAction("SetToGrenadeAttack", IE_Pressed, this, &AInputController::setStateToGrenadeAttack);
	InputComponent->BindAction("SetToHarvest", IE_Pressed, this, &AInputController::setStateToHarvest);
	InputComponent->BindAction("SetToPunchAttack", IE_Pressed, this, &AInputController::setStateToPunchAttack);

	PlayerCameraManagerClass = PlayerCameraManager->GetClass();
}

void AInputController::BeginPlay() {
	Super::BeginPlay();
	gameMode = Cast<ALaetusGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode) {
		grid = gameMode->getGameGrid();
	}
	FInputModeGameAndUI b;
	SetInputMode(b);
	SetShowMouseCursor(true);
}

/**
 * Initializes this InputController and sets up which ACrewController to 
 * distribute input to. Sets up the APlayerCameraManager reference so 
 * all controllers affect the same camera manager.
 * 
 * NOTE: If only one controller is provided, all input will 
 * be passed to that controller, and the AInputController::ChangeTurn function 
 * will only set the turn state to idle. If both controllers are provided, then
 * input  will be distributed to the ACrewController whose turn it currently is, 
 * and AInputController::ChangeTurn will switch and distribute input to the team 
 * whose turn is now becomes.
 * 
 * @param redController the ACrewController that is possessing the red team.
 * @param blueController the ACrewController that is possessing the blue team.
 */
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

/**
 * Change whose turn it is, and who receives input, to the provided team. 
 * 
 * NOTE: If this AInputController is providing input to two ACrewControllers, 
 * then this means that all input to this AInputController will now be forwarded 
 * to that team. Otherwise, this function will only set the turn state to Idle.
 */
void AInputController::changeTurn(int newTeam) {
	if (twoPlayers) {
		if (newTeam == 0) {
			currentTeamController = redTeamController;
			controlledCrew = currentTeamController->getControlledCrew();
			moveCameraToCrewMember();
		}else {
			currentTeamController = blueTeamController;
			controlledCrew = currentTeamController->getControlledCrew();
			moveCameraToCrewMember();
		}
	}
	setTurnState(Idle);
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
	//Clear current grid overlay
	if (grid) {
		grid->clearGridOverlay();
	}

	if (currentlySelectedTile) {
		//currentlySelectedTile->SetToRegularMaterial();
	}

	gameMode->EvaluateWin();// Check for winner + change turn if no one won (order subject to change)
}

/**
 * Toggle the currently selected crew member, set their turn state 
 * to Idle, and focus the camera on them.
 */
void AInputController::toggleCrewMember() {
	//Remove the yellow highlight on the HUD for the crew member that 
	//is no longer selected.
	gameMode->callHUDSetPlayer(-1);

	//Toggle and highlight character portrait on the HUDs
	controlledCrew->toggleSelectedCrewMember();
	gameMode->callHUDSetPlayer(controlledCrew->getSelectedCrewMemberIndex());

	//Now move the camera to focus on this crew member.
	//moveCameraToCrewMember();
	setStateToIdle();
}

///////////////////////// TURN STATE MACHINE /////////////////////////////////
/**
 * Set the state of the turn this player is in to the given state.
 *
 * @param newState the turn state this player is now entering.
 */
void AInputController::setTurnState(enum FTurnState newState) {
	currentTurnState = newState;
}

/** 
 * Set the current turn state to Movement, so input will be 
 * interpreted in the context of movement.
 */
void AInputController::setStateToMovement() {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("NOW IN MOVEMENT MODE"));
	if (grid) {
		grid->clearGridOverlay();
	}
	if (controlledCrew) {
		if (currentlySelectedTile) {
			//currentlySelectedTile->SetToRegularMaterial();
		}
		moveCameraSmoothly(controlledCrew->getCurrentCrewMember());
		setTurnState(Movement);
	}
}

/**
 * Set the current turn state to Movement, so input will be interpreted 
 * in the context of shooting the Rifle in one of four cardinal directions.
 */
void AInputController::setStateToRifleAttack() {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("NOW IN RIFLE ATTACK MODE"));
	if (grid) {
		grid->clearGridOverlay();
	}
	if (controlledCrew) {
		if (currentlySelectedTile) {
			//currentlySelectedTile->SetToRegularMaterial();
		}
		moveCameraSmoothly(controlledCrew->getCurrentCrewMember());
		const FVector2D origin = controlledCrew->getCurrentCrewMember()->getGridSpace()->getGridLocation();
		gameMode->getGameGrid()->colorGridDirectionsInRange(origin, 5);
		setTurnState(RifleAttack);
	}
}

/**
 * Set the current turn state to GrenadeAttack, so input will be interpreted
 * in the context of selecting a tile to throw a grenade to.
 */
void AInputController::setStateToGrenadeAttack() {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("NOW IN GRENADE ATTACK MODE"));
	if (grid) {
		grid->clearGridOverlay();
	}

	//Start by focusing on current tile. WASD will now move highlighted so player can select 
	//where to throw the grenade.
	if (controlledCrew) {
		currentlySelectedTile = controlledCrew->getCurrentCrewMember()->getGridSpace();
		moveCameraSmoothly(currentlySelectedTile);
		setTurnState(GrenadeAttack);
		gameMode->getGameGrid()->colorGridInRange(currentlySelectedTile->getGridLocation(), 2);
	}
}

/**
 * Set the current turn state to PunchAttack, so input will be interpreted
 * in the context of punching an ACrewMember in an adjacent AGridSpace.
 */
void AInputController::setStateToPunchAttack() {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("NOW IN PUNCH ATTACK MODE"));
	if (grid) {
		grid->clearGridOverlay();
	}
	if (controlledCrew) {
		if (currentlySelectedTile) {
			//currentlySelectedTile->SetToRegularMaterial();
		}
		moveCameraSmoothly(controlledCrew->getCurrentCrewMember());
		const FVector2D origin = controlledCrew->getCurrentCrewMember()->getGridSpace()->getGridLocation();
		gameMode->getGameGrid()->colorGridDirectionsInRange(origin, 1);
		setTurnState(PunchAttack);
	}
}

/**
 * Set the current turn state to Harvest, so input will be interpreted 
 * in the context of harvest resources if within range of a harvest source.
 */
void AInputController::setStateToHarvest() {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("NOW IN HARVEST MODE"));
	if (grid) {
		grid->clearGridOverlay();
	}

	if (controlledCrew) {
		if (currentlySelectedTile) {
			//currentlySelectedTile->SetToRegularMaterial();
		}
		moveCameraSmoothly(controlledCrew->getCurrentCrewMember());
		setTurnState(Harvest);
	}
}

/**
 * Set the current turn state to Movement, so input will be
 * interpreted in the context of selecting an action.
 */
void AInputController::setStateToIdle() {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("NOW IN IDLE MODE"));
	if (grid) {
		grid->clearGridOverlay();
	}

	if (controlledCrew) {
		if (currentlySelectedTile) {
			//currentlySelectedTile->SetToRegularMaterial();
		}
		moveCameraSmoothly(controlledCrew->getCurrentCrewMember());
		setTurnState(Idle);
	}
}

/**
 * Handle the "Up" key based on the current turn state this player is in.
 */
void AInputController::handleUp() {
	switch (currentTurnState) {
	case Idle:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing up in Idle state"));
		break;
	case CameraMovement:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing up in Camera state"));
		break;
	case Movement:
		moveIfValid(Direction::Up);
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
	case PunchAttack:
		price = 2;
		if (gameMode->checkLegalMove(price)) {
			currentTeamController->punch(DirectionToUnitVector(Direction::Up));
		}
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
	switch (currentTurnState) {
	case Idle:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing left in Idle state"));
		break;
	case CameraMovement:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing left in Camera state"));
		break;
	case Movement:
		moveIfValid(Direction::Left);
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
	case PunchAttack:
		price = 2;
		if (gameMode->checkLegalMove(price)) {
			currentTeamController->punch(DirectionToUnitVector(Direction::Left));
		}
		break;
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
	switch (currentTurnState) {
	case Idle:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing right in Idle state"));
		break;
	case CameraMovement:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing right in Camera state"));
		break;
	case Movement:
		moveIfValid(Direction::Right);
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
	case PunchAttack:
		price = 2;
		if (gameMode->checkLegalMove(price)) {
			currentTeamController->punch(DirectionToUnitVector(Direction::Right));
		}
		break;
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
	switch (currentTurnState) {
	case Idle:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing down in Idle state"));
		break;
	case CameraMovement:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing down in Camera state"));
		break;
	case Movement:
		moveIfValid(Direction::Down);
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
	case PunchAttack:
		price = 2;
		if (gameMode->checkLegalMove(price)) {
			currentTeamController->punch(DirectionToUnitVector(Direction::Down));
		}
		break;
		break;
	case Harvest:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing down in Harvest state"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing down in default state"));
	}
}

/**
 * Moves the current ACrewMember in the given direction only if the move is valid
 * (i.e. moving onto an available space) and they have enough action points left 
 * to do so.
 * 
 * @param direction the unit vector direction the current ACrewMember is moving in.
 */
void AInputController::moveIfValid(Direction direction) {
	//Price for movement is 1, if we are pushing core its 2
	//See if we are pushing core
	price = 1;
	FVector2D directionVector = DirectionToUnitVector(direction);

	ACrewMember* current = controlledCrew->getCurrentCrewMember();
	
	// Check for stickiness on the character itself or the character's tile
	bool stickyCharacter = (current->GetComponentByClass(UCharacterStickyEffect::StaticClass()) != nullptr) || (current->getGridSpace()->GetComponentByClass(UTileStickyEffect::StaticClass()) != nullptr);
	
	bool stickyCoreFragment = false;

	// Check for a core fragment
	ACoreFragment* coreFragment = controlledCrew->pushingCore(directionVector);
	if (coreFragment != nullptr) {
		
		// Add to price if pushing
		price += 1;

		// Check for stickiness on core fragment's tile
		stickyCoreFragment = (coreFragment->getGridSpace()->GetComponentByClass(UTileStickyEffect::StaticClass()) != nullptr);
	}

	// Add to price if character OR core fragement is stuck
	if (stickyCharacter || stickyCoreFragment)
		price += 1;

	bool canMove = grid->canMove(current->getGridSpace(), directionVector);
	if (canMove && gameMode->checkLegalMove(price)) {
		controlledCrew->moveSelectedCrewMember(directionVector);
	}
}

/**
 * Handle the "Confirm" input based on the current turn state this player is in.
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
 * Handles the "Pause" input.
 */
void AInputController::handlePause() {
	if (gameMode) {
		gameMode->callPauseMenuToggleVisibility();
	}
}

///////////////////////// CAMERA MOVEMENT /////////////////////////////////
/**
 * Move camera to the next AGridSpace in the specified direction.
 *
 * @param direction a Direction enum specifying the direction of
 *     the next tile to move the camera to.
 */
void AInputController::moveCameraToTile(Direction direction) {
	//Calculate the coordinate of the AGridSpace being moved to
	FVector2D directionVector = DirectionToUnitVector(direction);
	FVector2D currentLocation = currentlySelectedTile->getGridLocation();
	FVector2D newLocation = currentLocation + directionVector;

	//Check this target tile is within grenade-throwing range
	FVector2D origin = controlledCrew->getCurrentCrewMember()->getGridSpace()->getGridLocation();
	if (grid->areTilesWithinRange(origin, newLocation, 2)) {

		AGridSpace* newSpace = grid->getTile(newLocation);
		if (newSpace) {
			//Move camera and cursor to new tile
			currentlySelectedTile->RestoreOverlayColor();
			currentlySelectedTile = newSpace;
			currentlySelectedTile->SetOverlayToGreen(true);

			moveCameraSmoothly(currentlySelectedTile);
		}
	}
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