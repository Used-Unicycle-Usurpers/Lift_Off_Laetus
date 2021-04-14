// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Lift_Off_Laetus/Characters/Crew.h"
#include "Lift_Off_Laetus/GridSpace.h"
#include "LaetusGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LIFT_OFF_LAETUS_API ALaetusGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		int32 crewCount = 2;

	void ChangeTurn();
	int32 EvaluateWin();
	// ?? GetValidMoves(GridSpace);

private:

	// Game rule parameters
	int32 coresToWin = 3;  // Number of cores needed to win

	// Crew/turn parameters
	ACrew * crews;     // Array of crews, defined at runtime
	int32 currentCrew = 0;          // Which crew is playing currently
	// TimerHandle turnTimer;     // Keeps track of the time left in the turn
	// TurnActionStack * turnStack;  // Records the actions taken during the current turn

	// Board parameters
	AGridSpace * gridSpaces; // 2D array, defined at runtime
	
	// Private helper methods
	void ClearTurnActionStack(); // Clears the stack of actions done on the current crew's turn

};
