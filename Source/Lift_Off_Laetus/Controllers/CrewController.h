// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CrewController.generated.h"

/**
 * 
 */
UCLASS()
class LIFT_OFF_LAETUS_API ACrewController : public APlayerController {
	GENERATED_BODY()

public:
	ACrewController();

	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	/**
	 * Sets up the APlayerCameraManager reference so all controllers affect the
	 * same camera manager.
	 */
	void init(class ACrew* newControlledCrew, class AInputController* newInputController);
	
	void enableInputController();
	void disableInputController();

	/**
	 * Sets up the APlayerCameraManager reference so all controllers affect the
	 * same camera manager.
	 */
	void moveCameraToCrewMember();

	/**
	 * Moves the camera to the current ACrewMember.
	 */
	void toggleCrewMember();
	
	//The PlayerCameraManager that both Crews refernce to move the 
	//shared camera.
	class APlayerCameraManager* cameraManager;

	//A reference to the game mode for quick access.
	class ALaetusGameMode* gameMode;

	/**
	 * Have the currently selected crew member shoot their rifle in the given direction.
	 */
	void shoot(FVector2D direction);

	/**
	 * Have the currently selected crew member launch a grenade in the given direction.
	 */
	void launch(FVector2D target);

	/**
	 * Move camera to the next AGridSpace in the specified direction.
	 */
	void moveCameraToTile(enum Direction direction);

	/**
	 * The tile that is currently being highlighted in GrenadeAttack mode.
	 */
	class AGridSpace* currentlySelectedTile;

	/**
	 * Move the camera smoothly from its current location to the target actor.
	 */
	void moveCameraSmoothly(AActor* target);

	class ACrew* getControlledCrew();

private:
	class ACrew* controlledCrew;
	class AInputController* inputController;
	void setControlledCrew(class ACrew* newControlledCrew);
	void setInputController(class AInputController* newInputController);
};