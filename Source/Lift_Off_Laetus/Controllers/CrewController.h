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

};
