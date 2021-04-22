// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LaetusGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LIFT_OFF_LAETUS_API ALaetusGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALaetusGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		int32 crewCount = 2;

	void ChangeTurn();
	int32 EvaluateWin();
	void BeginNewTurn();
	// ?? GetValidMoves(GridSpace);

private:

	// Game rule parameters
	int32 coresToWin = 3;  // Number of cores needed to win

	// Crew/turn parameters
	//ACrew* crews;     // Array of crews, defined at runtime
	TArray<class ACrew*> crews; // for testing

	int32 currentCrew = 0;          // Which crew is playing currently
	// TimerHandle turnTimer;     // Keeps track of the time left in the turn
	// TurnActionStack * turnStack;  // Records the actions taken during the current turn

	//The grid representing the tiles of the map.
	UPROPERTY(EditAnywhere, Category = MyCategory)
		class AGrid* grid;

	class ACrewController* redTeamController;
	class ACrewController* blueTeamController;

	// Private helper methods
	void ClearTurnActionStack(); // Clears the stack of actions done on the current crew's turn

	virtual APawn* SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot);
	virtual UClass* GetDefaultPawnClassForController(AController* InController);
};
