// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Components/SlateWrapperTypes.h"
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

USTRUCT()
struct FSetEffectOverlayParams {
	GENERATED_BODY()
	int teamIndex;
	int playerIndex;
	UTexture2D* overlayTexture;
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

//Params used to call setInputControllers in the UI.
USTRUCT()
struct FsetInputControllers {
	GENERATED_BODY()
		class AInputController* controller1;
		class AInputController* controller2;
		bool twoInputs;
};

//Params used to call setCrews in the UI.
USTRUCT()
struct FsetCrews {
	GENERATED_BODY()
		class ACrew* redCrew;
		class ACrew* blueCrew;
};

//Params used to play the winning team cinematic.
USTRUCT()
struct FplayWinningVideo {
	GENERATED_BODY()
		int winningTeam;
};

//Params used to set the visibility of the throw grenade 
//instruction in the HUD.
USTRUCT()
struct FsetThrowGrenadeVisibility {
	GENERATED_BODY()
		ESlateVisibility visibility;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UUserWidget> VictoryVideoWidgetClass;

	UPROPERTY()
		class UUserWidget* hud;

	UPROPERTY()
		class UUserWidget* pauseMenu;

	UPROPERTY()
		class UUserWidget* victoryVideo;

	class AGrid* getGameGrid();

	void callHUDSetPlayer(int newPlayerIndex);
	void callHUDSetEffectOverlay(int teamIndex, int playerIndex, UTexture2D* overlayTexture);

	//for action bar
	void callHUDUpdateAB(int32 status);
	void callHUDMessage(bool visible, int32 message);
	void callHUDTimer();
	int32 getABStatus();
	bool checkLegalMove(int32 actionPrice);

	void callPauseMenuToggleVisibility();

	void callHUDToggleThrowGrenadeInstruction(ESlateVisibility newVisibility);

private:

	// Game rule parameters
	int32 coresToWin = 3;  // Number of cores needed to win

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

	bool singleInput;

	void OnGameEnd(int32 winner);

	FTimerHandle ActionPriceTimerHandle;
	FTimerHandle NoPointsTimerHandle;
	FTimerHandle ChangeTurnTimerHandle;
	int32 message;
	bool visible;
	bool firstChangeTurn = true; //so message does not show up during startup

	void callHUDSetInputControllers(class AInputController* c1, class AInputController* c2, bool singleInputOnly);

	void callHUDSetCrews(class ACrew* c1, class ACrew* c2);

	void callVictoryVideoPlayWinningVideo(int team);

	//Sound that is played while changing turns.
	class USoundCue* changingTurnSound;

	//Sound that is played when someone tries to take an action 
	//for which they do not have enough action points.
	class USoundCue* errorSound;
};
