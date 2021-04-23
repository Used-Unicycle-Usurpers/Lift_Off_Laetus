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
	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	UFUNCTION()
		void testBinding();

	void enable();
	void disable();

	void endTurn();

	/**
	 * Sets up the APlayerCameraManager reference so all controllers affect the
	 * same camera manager.
	 */
	void moveCameraToCrewMember();

	/**
	 * Moves the camera to the current ACrewMember.
	 */
	void toggleCrewMember();
	
	/**
	 * Toggle the currently selected crew member and focus the camera
	 * on them.
	 */
	void initCamera();

	//The PlayerCameraManager that both Crews refernce to move the 
	//shared camera.
	class APlayerCameraManager* cameraManager;
};
