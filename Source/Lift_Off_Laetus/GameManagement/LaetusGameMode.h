// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <string>
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

//Params used to update the action bar in the UI
USTRUCT()
struct FsetABParams {
	GENERATED_BODY()
		int32 status;
};

//Params used to display messages on HUD for a certain amount of seconds
USTRUCT()
struct FsetHUDMessageParams {
	GENERATED_BODY() 
		bool visible;
		int32 message;
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
	
	//for action bar
	void callHUDUpdateAB(int32 status);
	void callHUDMessage(bool visible, int32 message);
	void callHUDTimer();
	int32 getABStatus();
	bool checkLegalMove(int32 actionPrice);


private:

	// Game rule parameters
	int32 coresToWin = 1;  // Number of cores needed to win

	// Crew/turn parameters
	//ACrew* crews;     // Array of crews, defined at runtime
	TArray<class ACrew*> crews; // for testing

	int32 currentCrew = 0;          // Which crew is playing currently
	// TimerHandle turnTimer;     // Keeps track of the time left in the turn
	// TurnActionStack * turnStack;  // Records the actions taken during the current turn
	int32 actionbar = 10;

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

	FTimerHandle TimerHandle;
	int32 message;
	bool visible;
	bool firstChangeTurn = true; //so message does not show up during startup
};
