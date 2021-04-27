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
	void init();

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
	 * Have the currently selected crew member shoot their rifle in the given direction.
	 */
	void shoot(FVector2D direction);

	//Helper functions for shoot, to specify the direction to shoot in.
	void shootUp();
	void shootLeft();
	void shootRight();
	void shootDown();
	
	/**
	 * Have the currently selected crew member launch a grenade in the given direction.
	 */
	void launch(FVector2D direction);

	//Helper functions for launch, to specify the direction to throw 
	//a grenade in.
	void launchUp();
	void launchLeft();
	void launchRight();
	void launchDown();
	
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

	// Each moves the selected character in the corresponding direction
	void moveCrewMemberRight();
	void moveCrewMemberLeft();
	void moveCrewMemberTowardScreen();
	void moveCrewMemberAwayFromScreen();

	virtual void OnPossess(APawn* InPawn) override;

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

	//A reference to the game mode for quick access.
	class ALaetusGameMode* gameMode;

	void moveCameraToTile(FVector2D direction);

	class AGridSpace* currentlySelectedTile;

	void moveCameraSmoothly(AActor* target);
};
