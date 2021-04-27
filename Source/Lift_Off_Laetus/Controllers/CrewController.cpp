// Fill out your copyright notice in the Description page of Project Settings.

#include "CrewController.h"
#include "../Characters/Crew.h"
#include "../Characters/CrewMember.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "../GameManagement/LaetusGameMode.h"
#include "../GameManagement/GridSpace.h"
#include "../GameManagement/Grid.h"

ACrewController::ACrewController() {
	currentTurnState = Movement;
}

void ACrewController::BeginPlay() {
	gameMode = Cast<ALaetusGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void ACrewController::SetupInputComponent() {
	Super::SetupInputComponent();
	disable();//Let game mode enable input based on whose turn it is.

	//Inputs for swithcing between crew members and turns
	InputComponent->BindAction("ToggleCrewMember", IE_Pressed, this, &ACrewController::toggleCrewMember);
	InputComponent->BindAction("EndTurn", IE_Pressed, this, &ACrewController::endTurn);

	//These are the basic 4 controls for moving and attacking. They will be interpreteed differently 
	//depending on what turn state the player is currently in.
	InputComponent->BindAction("Up", IE_Pressed, this, &ACrewController::handleUp);
	InputComponent->BindAction("Left", IE_Pressed, this, &ACrewController::handleLeft);
	InputComponent->BindAction("Right", IE_Pressed, this, &ACrewController::handleRight);
	InputComponent->BindAction("Down", IE_Pressed, this, &ACrewController::handleDown);

	//For debugging, these keybinds allow you to manually set the state, rather then selecting move, 
	//attack, or collect on the UI
	InputComponent->BindAction("SetToMovement", IE_Pressed, this, &ACrewController::setStateToMovement);
	InputComponent->BindAction("SetToRifleAttack", IE_Pressed, this, &ACrewController::setStateToRifleAttack);
	InputComponent->BindAction("SetToGrenadeAttack", IE_Pressed, this, &ACrewController::setStateToGrenadeAttack);
	InputComponent->BindAction("SetToHarvest", IE_Pressed, this, &ACrewController::setStateToHarvest);

	PlayerCameraManagerClass = PlayerCameraManager->GetClass();
}

/**
 * Sets up the APlayerCameraManager reference so all controllers affect the
 * same camera manager.
 */
void ACrewController::init() {
	cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
}

/** 
 * Enable input on this controller.
 */
void ACrewController::enable() {
	EnableInput(this);
}

/**
 * Disable input on this controller.
 */
void ACrewController::disable() {
	DisableInput(this);
}

/**
 * Tell the game mode to end this players turn and switch control over to 
 * the other player.
 */
void ACrewController::endTurn() {
	gameMode->ChangeTurn();
}

/**
 * Moves the camera to the current ACrewMember.
 */
void ACrewController::moveCameraToCrewMember() {
	ACrew* crew = Cast<ACrew>(GetPawn());
	if (crew) {
		ACrewMember* current = crew->getCurrentCrewMember();
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
void ACrewController::toggleCrewMember() {
	//Remove the yellow highlight on the HUD for the crew member that 
	//is no longer selected.
	gameMode->callHUDSetPlayer(-1);

	//Toggle and highlight character portrait on the HUD
	ACrew* c = Cast<ACrew>(GetPawn());
	c->toggleSelectedCrewMember();
	gameMode->callHUDSetPlayer(c->getSelectedCrewMemberIndex());

	//Now move the camera to focus on this crew member.
	moveCameraToCrewMember();
}

/**
 * Set the state of the turn this player is in to the given state.
 * 
 * @param newState the turn state this player is now entering.
 */
void ACrewController::setTurnState(enum FTurnState newState) {
	currentTurnState = newState;
}

void ACrewController::setStateToMovement() {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("NOW IN MOVEMENT MODE"));
	ACrew* crew = Cast<ACrew>(GetPawn());
	if (crew) {
		if (currentlySelectedTile) {
			currentlySelectedTile->SetToRegularMaterial();
		}
		moveCameraSmoothly(crew->getCurrentCrewMember());
		setTurnState(Movement);
	}
}

void ACrewController::setStateToRifleAttack() {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("NOW IN RIFLE ATTACK MODE"));
	ACrew* crew = Cast<ACrew>(GetPawn());
	if (crew) {
		if (currentlySelectedTile) {
			currentlySelectedTile->SetToRegularMaterial();
		}
		moveCameraSmoothly(crew->getCurrentCrewMember());
		setTurnState(RifleAttack);
	}
}

void ACrewController::setStateToGrenadeAttack() {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("NOW IN GRENADE ATTACK MODE"));
	
	//Start by focusing on current tile. WASD will now move highlighted so player can select 
	//where to throw the grenade.
	ACrew* crew = Cast<ACrew>(GetPawn());
	if (crew) {
		currentlySelectedTile = crew->getCurrentCrewMember()->getGridSpace();
		moveCameraSmoothly(currentlySelectedTile);
		setTurnState(GrenadeAttack);
	}
}

void ACrewController::setStateToHarvest() {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("NOW IN HARVEST MODE"));
	ACrew* crew = Cast<ACrew>(GetPawn());
	if (crew) {
		if (currentlySelectedTile) {
			currentlySelectedTile->SetToRegularMaterial();
		}
		moveCameraSmoothly(crew->getCurrentCrewMember());
		setTurnState(Harvest);
	}
}

/**
 * Handle the "Up" key based on the current turn state this player is in.
 */
void ACrewController::handleUp() {
	switch (currentTurnState) {
	case Idle:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing up in Idle state"));
		break;
	case CameraMovement:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing up in Camera state"));
		break;
	case Movement:
		moveCrewMemberAwayFromScreen();
		break;
	case RifleAttack:
		shootUp();
		break;
	case GrenadeAttack:
		moveCameraToTile(FVector2D(-1, 0));
		break;
	case Harvest:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing up in Harvest state"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing up in default state"));
	}
}

/**
 * Handle the "Left" input based on the current turn state this player is in.
 */
void ACrewController::handleLeft() {
	switch (currentTurnState) {
	case Idle:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing left in Idle state"));
		break;
	case CameraMovement:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing left in Camera state"));
		break;
	case Movement:
		moveCrewMemberLeft();
		break;
	case RifleAttack:
		shootLeft();
		break;
	case GrenadeAttack:
		moveCameraToTile(FVector2D(0, -1));
		break;
	case Harvest:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing left in Harvest state"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing up in default state"));
	}
}

/**
 * Handle the "Right" input based on the current turn state this player is in.
 */
void ACrewController::handleRight() {
	switch (currentTurnState) {
	case Idle:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing right in Idle state"));
		break;
	case CameraMovement:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing right in Camera state"));
		break;
	case Movement:
		moveCrewMemberRight();
		break;
	case RifleAttack:
		shootRight();
		break;
	case GrenadeAttack:
		moveCameraToTile(FVector2D(0, 1));
		break;
	case Harvest:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing right in Harvest state"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing up in default state"));
	}
}

/**
 * Handle the "Down" input based on the current turn state this player is in.
 */
void ACrewController::handleDown() {
	switch (currentTurnState) {
	case Idle:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing down in Idle state"));
		break;
	case CameraMovement:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing down in Camera state"));
		break;
	case Movement:
		moveCrewMemberTowardScreen();
		break;
	case RifleAttack:
		shootDown();
		break;
	case GrenadeAttack:
		moveCameraToTile(FVector2D(1, 0));
		break;
	case Harvest:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing down in Harvest state"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("No actions for pressing up in default state"));
	}
}

/**
 * Have the currently selected crew member shoot their rifle in the given direction.
 * 
 * @param direction the carindal direction for the currently selected ACrewMember 
 *     to fire their rifle towards.
 */
void ACrewController::shoot(FVector2D direction) {
	ACrew* crew = Cast<ACrew>(GetPawn());
	if (crew) {
		crew->getCurrentCrewMember()->Shoot(direction, true);
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Tried to shoot rifle, but controlled Crew pawn was null for controller %s"), *GetName());
	}
}

void ACrewController::shootUp() {
	shoot(FVector2D(-1, 0));
}

void ACrewController::shootLeft() {
	shoot(FVector2D(0, -1));
}

void ACrewController::shootRight() {
	shoot(FVector2D(0, 1));
}

void ACrewController::shootDown() {
	shoot(FVector2D(1, 0));
}

/**
 * Have the currently selected crew member launch a grenade in the given direction.
 *
 * @param direction the carindal direction for the currently selected ACrewMember
 *     to launch a grenade towards.
 */
void ACrewController::launch(FVector2D direction) {
	ACrew* crew = Cast<ACrew>(GetPawn());
	if (crew) {
		crew->getCurrentCrewMember()->Shoot(direction, false);
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Tried to launch a grenade, but controlled Crew pawn was null for controller %s"), *GetName());
	}
}

void ACrewController::launchUp() {
	launch(FVector2D(-1, 0));
}

void ACrewController::launchLeft() {
	launch(FVector2D(0, -1));
}

void ACrewController::launchRight() {
	launch(FVector2D(0, 1));
}

void ACrewController::launchDown() {
	launch(FVector2D(1, 0));
}

/**
* Move the currently selected crew member in to the right
*/
void ACrewController::moveCrewMemberRight() {
	ACrew* c = Cast<ACrew>(GetPawn());
	c->moveSelectedCrewMember(FVector2D(0, 1));
}

/**
* Move the currently selected crew member in to the right
*/
void ACrewController::moveCrewMemberLeft() {
	ACrew* c = Cast<ACrew>(GetPawn());
	c->moveSelectedCrewMember(FVector2D(0, -1));
}

/**
* Move the currently selected crew member in to the right
*/
void ACrewController::moveCrewMemberTowardScreen() {
	ACrew* c = Cast<ACrew>(GetPawn());
	c->moveSelectedCrewMember(FVector2D(1, 0));
}

/**
* Move the currently selected crew member in to the right
*/
void ACrewController::moveCrewMemberAwayFromScreen() {
	ACrew* c = Cast<ACrew>(GetPawn());
	c->moveSelectedCrewMember(FVector2D(-1, 0));
}

void ACrewController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	ACrew* c = Cast<ACrew>(InPawn);
	if (c) {
		c->setController(this);
	}
}

void ACrewController::moveCameraToTile(FVector2D direction) {
	AGrid* grid = gameMode->getGameGrid();

	FVector2D currentLocation = currentlySelectedTile->getGridLocation();
	FVector2D newLocation = currentLocation + direction;

	AGridSpace* newSpace = grid->getTile(newLocation);
	if (newSpace) {
		currentlySelectedTile->SetToRegularMaterial();
		currentlySelectedTile = newSpace;
		currentlySelectedTile->SetToGreen();

		moveCameraSmoothly(currentlySelectedTile);
	}
}

void ACrewController::moveCameraSmoothly(AActor* target) {
	FViewTargetTransitionParams p;
	p.BlendFunction = EViewTargetBlendFunction::VTBlend_Linear;
	p.BlendTime = 0.1f;
	cameraManager->SetViewTarget(target, p);
}