// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LaetusGameMode.generated.h"


//Params used to call the setTeam() function in the UI
USTRUCT()
struct FsetTeamParams {
	GENERATED_BODY()
	int teamIndex;
};

//Params used to call the setTeam() function in the UI
USTRUCT()
struct FsetPlayerParams {
	GENERATED_BODY()
		int playerIndex;
};

enum FTurnState {
	Idle,
	CameraMovement,
	Movement,
	RifleAttack,
	GrenadeAttack,
	Harvest
};

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
	void EvaluateWin();
	// ?? GetValidMoves(GridSpace);

	class APlayerCameraManager* cameraManager;

	UPROPERTY(EditAnywhere)
		class UCameraComponent* camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY()
		class UUserWidget* hud;

	class AGrid* getGameGrid();

	void callHUDSetPlayer(int newPlayerIndex);

private:

	// Game rule parameters
	int32 coresToWin = 1;  // Number of cores needed to win

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
	class AInputController* inputController;
	class AInputController* inputController2;

	// Private helper methods
	void ClearTurnActionStack(); // Clears the stack of actions done on the current crew's turn

	virtual APawn* SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot);
	virtual UClass* GetDefaultPawnClassForController(AController* InController);

	void OnGameEnd(int32 winner);

	bool singleInput;
};
