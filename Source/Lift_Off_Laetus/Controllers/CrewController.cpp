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
	setControlledCrew(newControlledCrew);
	setInputController(newInputController);
}

/**
 * Enables input on the AInputController that is providing input to this ACrewController.
 */
void ACrewController::enableInputController() {
	if (inputController) {
		inputController->enable();
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Input Controller was null when trying to enable input from: %s"), *GetName());
	}
}

/**
 * Disables input on the AInputController that is providing input to this ACrewController.
 */
void ACrewController::disableInputController() {
	if (inputController) {
		inputController->disable();
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Input Controller was null when trying to disable input from: %s"), *GetName());
	}
}

/**
 * Have the currently selected crew member shoot their rifle in the given direction.
 *
 * @param direction the cardinal direction for the currently selected ACrewMember
 *     to fire their rifle towards.
 */
void ACrewController::shoot(FVector2D direction) {
	if (controlledCrew) {
		controlledCrew->getCurrentCrewMember()->Shoot(direction, true);
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Tried to shoot rifle, but controlledCrew pawn was null for controller %s"), *GetName());
	}
}

/**
 * Have the currently selected crew member launch a grenade in the given direction.
 *
 * @param direction the cardinal direction for the currently selected ACrewMember
 *     to launch a grenade towards.
 */
void ACrewController::launch(FVector2D target) {
	if (controlledCrew) {
		inputController->moveCameraSmoothly(controlledCrew->getCurrentCrewMember());
		controlledCrew->getCurrentCrewMember()->Shoot(target, false);
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Tried to launch a grenade, but controlledCrew pawn was null for controller %s"), *GetName());
	}
}

/**
 * Have the currently selected crew member punch at the ACrewMember in
 * the adjacent cell in the given direction.
 * 
 * @param direction the cardinal direction for the currently selected ACrewMember
 *    to punch towards.
 */
void ACrewController::punch(FVector2D direction) {
	if (controlledCrew) {
		controlledCrew->getCurrentCrewMember()->Punch(direction);
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Tried to punch, but controlledCrew pawn was null for controller %s"), *GetName());
	}
}

/**
 * Returns the ACrew that is currently controlled by this ACrewController.
 */
ACrew* ACrewController::getControlledCrew() {
	return controlledCrew;
}

/**
 * Returns the AInputController that is currently forwarding inputs to
 * this ACrewController.
 */
AInputController* ACrewController::getInputController() {
	return inputController;
}

/**
 * Sets the ACrew being controlled to the provided ACrew.
 * NOTE: This NOT the same as Possess(), this only provides a refernce
 * to the ACrew to this ACrewController.
 *
 * @param newControlledCrew the ACrew the is being controlled by this ACrewController.
 */
void ACrewController::setControlledCrew(ACrew* newControlledCrew) {
	controlledCrew = newControlledCrew;
}

/**
 * Sets the AInputController that is forwarding inputs to this ACrewController to
 * the provided AInputController.
 *
 * @param newInputController the AInputController the is forwarding inputs to this
 *     ACrewController.
 */
void ACrewController::setInputController(AInputController* newInputController) {
	inputController = newInputController;
}
