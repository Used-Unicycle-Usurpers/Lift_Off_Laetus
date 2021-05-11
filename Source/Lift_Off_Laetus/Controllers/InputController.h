// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../GameManagement/GameEnums.h"
#include "InputController.generated.h"

/**
 * Controller for taking-in and interpreting player input based on the 
 * current turn state.
 */
UCLASS()
class LIFT_OFF_LAETUS_API AInputController : public APlayerController {
	GENERATED_BODY()

public:
	//A reference to the game mode for quick access.
	class ALaetusGameMode* gameMode;

	class AGrid* grid;

	AInputController();

	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

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
	 */
	void init(class ACrewController* redController, ACrewController* blueController);

	/**
	 * Change whose turn it is, and who receives input, to the provided team.
	 *
	 * NOTE: If this AInputController is providing input to two ACrewControllers,
	 * then this means that all input to this AInputController will now be forwarded
	 * to that team. Otherwise, this function will only set the turn state to Idle.
	 */
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
	 * Toggle the currently selected crew member, set their turn state
	 * to Idle, and focus the camera on them.
	 */
	void toggleCrewMember();

	///////////////////////// TURN STATE MACHINE /////////////////////////////////
	//The current state of their turn this player is currently in.
	enum FTurnState currentTurnState;

	/**
	 * Set the state of the turn this player is in to the given state.
	 */
	void setTurnState(enum FTurnState newState);

	/**
	 * Set the current turn state to Movement, so input will be
	 * interpreted in the context of movement.
	 */
	void setStateToMovement();

	/**
	 * Set the current turn state to Movement, so input will be interpreted
	 * in the context of shooting the Rifle in one of four cardinal directions.
	 */
	void setStateToRifleAttack();

	/**
	 * Set the current turn state to Movement, so input will be interpreted
	 * in the context of selecting a tile to throw a grenade to.
	 */
	void setStateToGrenadeAttack();

	/**
	 * Set the current turn state to Harvest, so input will be interpreted
	 * in the context of harvest resources if within range of a harvest source.
	 */
	void setStateToHarvest();

	/**
	 * Set the current turn state to Movement, so input will be
	 * interpreted in the context of selecting an action.
	 */
	void setStateToIdle();

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
	 * Moves the current ACrewMember in the given direction only if the move is valid
	 * (i.e. moving onto an available space) and they have enough action points left
	 * to do so.
	 */
	void moveIfValid(Direction direction);

	/**
	 * Handle the "Confim" input based on the current turn state this player is in.
	 */
	void handleConfirm();

	///////////////////////// CAMERA MOVEMENT /////////////////////////////////
	//The PlayerCameraManager that both Crews refernce to move the 
	//shared camera.
	class APlayerCameraManager* cameraManager;

	/**
	 * Move camera to the next AGridSpace in the specified direction.
	 */
	void moveCameraToTile(enum Direction direction);

	/**
	 * The tile that is currently being highlighted in GrenadeAttack mode.
	 */
	class AGridSpace* currentlySelectedTile;

	/**
	 * Sets up the APlayerCameraManager reference so all controllers affect the
	 * same camera manager.
	 */
	void moveCameraToCrewMember();

	/**
	 * Move the camera smoothly from its current location to the target actor.
	 */
	void moveCameraSmoothly(AActor* target);

private:
	//The ACrewController that is possessing the red team ACrew
	class ACrewController* redTeamController;

	//The ACrewController that is possessing the blue team ACrew
	class ACrewController* blueTeamController;

	//The ACrewController that is possessing the ACrew whose turn it currently is.
	class ACrewController* currentTeamController;

	//The ACrew is turn it currently is.
	class ACrew* controlledCrew;

	//True if disitributing input for two players on the same input 
	//device (i.e. keyboard), false otherwise
	bool twoPlayers;

	//The price (in action points) of the next action to be taken 
	//for the action bar
	int32 price = 0;
};