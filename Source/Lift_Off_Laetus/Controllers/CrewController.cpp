// Fill out your copyright notice in the Description page of Project Settings.

#include "CrewController.h"
#include "../Characters/Crew.h"
#include "../Characters/CrewMember.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "../GameManagement/LaetusGameMode.h"
#include "../GameManagement/GridSpace.h"
#include "../GameManagement/Grid.h"
#include "../Controllers/InputController.h"

ACrewController::ACrewController() {
	
}

void ACrewController::BeginPlay() {
	Super::BeginPlay();
	gameMode = Cast<ALaetusGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void ACrewController::SetupInputComponent() {
	Super::SetupInputComponent();

	//TODO: Move this somewhere else?
	PlayerCameraManagerClass = PlayerCameraManager->GetClass();
}

/**
 * Sets up the APlayerCameraManager reference so all controllers affect the
 * same camera manager.
 */
void ACrewController::init(ACrew* newControlledCrew, AInputController* newInputController) {
	cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	setControlledCrew(newControlledCrew);
	setInputController(newInputController);
}

void ACrewController::enableInputController() {
	if (inputController) {
		inputController->enable();
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Input Controller was null when trying to enable input from: %s"), *GetName());
	}
}

void ACrewController::disableInputController() {
	if (inputController) {
		inputController->disable();
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Input Controller was null when trying to disable input from: %s"), *GetName());
	}
}

void ACrewController::setControlledCrew(ACrew* newControlledCrew) {
	controlledCrew = newControlledCrew;
}

void ACrewController::setInputController(AInputController* newInputController) {
	inputController = newInputController;
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
 * Have the currently selected crew member shoot their rifle in the given direction.
 *
 * @param direction the carindal direction for the currently selected ACrewMember
 *     to fire their rifle towards.
 */
void ACrewController::shoot(FVector2D direction) {
	if (controlledCrew) {
		controlledCrew->getCurrentCrewMember()->Shoot(direction, true);
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Tried to shoot rifle, but controlled Crew pawn was null for controller %s"), *GetName());
	}
}

/**
 * Have the currently selected crew member launch a grenade in the given direction.
 *
 * @param direction the carindal direction for the currently selected ACrewMember
 *     to launch a grenade towards.
 */
void ACrewController::launch(FVector2D target) {
	if (controlledCrew) {
		moveCameraSmoothly(controlledCrew->getCurrentCrewMember());
		controlledCrew->getCurrentCrewMember()->Shoot(target, false);
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Tried to launch a grenade, but controlled Crew pawn was null for controller %s"), *GetName());
	}
}


/**
 * Move camera to the next AGridSpace in the specified direction.
 * 
 * @param direction a Direction enum specifying the direction of 
 *     the next tile to move the camera to.
 */
void ACrewController::moveCameraToTile(Direction direction) {
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

	AGridSpace* newSpace = grid->getTile(newLocation);
	if (newSpace) {
		currentlySelectedTile->SetToRegularMaterial();
		currentlySelectedTile = newSpace;
		currentlySelectedTile->SetToGreen();

		moveCameraSmoothly(currentlySelectedTile);
	}
}

/**
 * Move the camera smoothly from its current location to the target actor.
 * 
 * @param target the actor to smoothly move the camera to.
 */
void ACrewController::moveCameraSmoothly(AActor* target) {
	FViewTargetTransitionParams p;
	p.BlendFunction = EViewTargetBlendFunction::VTBlend_Linear;
	p.BlendTime = 0.1f;
	cameraManager->SetViewTarget(target, p);
}

ACrew* ACrewController::getControlledCrew() {
	return controlledCrew;
}