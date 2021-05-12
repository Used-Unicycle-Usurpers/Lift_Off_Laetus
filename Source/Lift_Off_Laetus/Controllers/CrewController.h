// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CrewController.generated.h"

/**
 * Controller that possesses an ACrew an forwards input to the currently 
 * selected ACrewMember.
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
	
	/**
	 * Enables input on the AInputController that is providing input to this ACrewController.
	 */
	void enableInputController();

	/**
	 * Disables input on the AInputController that is providing input to this ACrewController.
	 */
	void disableInputController();
	
	/**
	 * Have the currently selected crew member shoot their rifle in the given direction.
	 */
	void shoot(FVector2D direction);

	/**
	 * Have the currently selected crew member launch a grenade in the given direction.
	 */
	void launch(FVector2D target);

	/**
	 * Have the currently selected crew member punch at the ACrewMember in 
	 * the adjacent cell in the given direction.
	 */
	void punch(FVector2D direction);

	/**
	 * Returns the ACrew that is currently controlled by this ACrewController.
	 */
	class ACrew* getControlledCrew();

	/**
	 * Returns the AInputController that is currently forwarding inputs to
	 * this ACrewController.
	 */
	class AInputController* getInputController();

private:
	//The ACrew this is currently being controlled by this ACrewController.
	class ACrew* controlledCrew;

	//The AInputController that is forwarding inputs to this ACrewController.
	class AInputController* inputController;

	/**
	 * Sets the ACrew being controlled to the provided ACrew.
	 * NOTE: This NOT the same as Possess(), this only provides a refernce
	 * to the ACrew to this ACrewController.
	 */
	void setControlledCrew(class ACrew* newControlledCrew);

	/**
	 * Sets the AInputController that is forwarding inputs to this ACrewController to
	 * the provided AInputController.
	 */
	void setInputController(class AInputController* newInputController);
};