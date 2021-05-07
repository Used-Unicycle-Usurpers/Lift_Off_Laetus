// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputController.generated.h"

/**
 * 
 */
UCLASS()
class LIFT_OFF_LAETUS_API AInputController : public APlayerController {
	GENERATED_BODY()

public:
	AInputController();
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

	/**
	 * Sets up the APlayerCameraManager reference so all controllers affect the
	 * same camera manager.
	 * TODO: Add info on parameters
	 */
	void init(class ACrewController* redController, ACrewController* blueController);

	void changeTurn(int newTeam);


	/**
	 * Enable input on this controller.
	 */
	void enable();

	/**
	 * Disable input on this controller.
	 */
	void disable();

	/**
	 * Tell the game mode to end this players turn and switch control over to
	 * the other player.
	 */
	void endTurn();

	/**
	 * Handle the "Up" input based on the current turn state this player is in.
	 */
	void handleUp();

	/**
	 * Handle the "Left" input based on the current turn state this player is in.
	 */
	void handleLeft();

	/**
	 * Handle the "Right" input based on the current turn state this player is in.
	 */
	void handleRight();

	/**
	 * Handle the "Down" input based on the current turn state this player is in.
	 */
	void handleDown();

	/**
	 * Handle the "Confim" input based on the current turn state this player is in.
	 */
	void handleConfirm();

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

	//virtual void OnPossess(APawn* InPawn) override;

	//The current state of their turn this player is currently in.
	enum FTurnState currentTurnState;

	/**
	 * Set the state of the turn this player is in to the given state.
	 */
	void setTurnState(enum FTurnState newState);

	//Helper functions for setTurnState to specify the state to switch to.
	void setStateToMovement();
	void setStateToRifleAttack();
	void setStateToGrenadeAttack();
	void setStateToHarvest();
	void setStateToIdle();

	//A reference to the game mode for quick access.
	class ALaetusGameMode* gameMode;

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

private:
	class ACrewController* redTeamController;
	class ACrewController* blueTeamController;
	class ACrewController* currentTeamController;
	class ACrew* controlledCrew;

};
