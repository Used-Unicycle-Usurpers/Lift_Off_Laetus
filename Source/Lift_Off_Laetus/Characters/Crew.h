// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../GameManagement/GameEnums.h"
#include "Crew.generated.h"

UCLASS()
class LIFT_OFF_LAETUS_API ACrew : public APawn {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACrew(); 
	
	//void SetTeam(int32 newTeam);
	void SetUp(Team newTeam, class AGrid* newGrid, class ACrewController* newController);

	//Action bar stuff 
	int32 GetActionBarStatus();
	void UpdateActionBar(int32 update);

	//Return the location of the first crewMember 
	FVector GetStartingLocation();

	void setController(class ACrewController* newController);

	//return if we are pushing a core piece 
	bool pushingCore(FVector2D direction);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	TArray<class ACrewMember*> crewMembers; // for testing

	/**
	 * Returns a reference to the currently selected ACrewMember.
	 */
	ACrewMember* getCurrentCrewMember();

	/**
	 * Toggles the currently selected ACrewMember
	 */
	int toggleSelectedCrewMember();

	void setSelectedCrewMember(int current);

	/**
	* Move the currently selected ACrewMember relative to its current location
	*/
	void moveSelectedCrewMember(FVector2D direction);

	int32 getSelectedCrewMemberIndex();

	int32 getCoreCount();

	void incrementCores();

private:
	// Array of crew members 
	//class ACrewMember* crewMembers[3] = {};

	// Index of current crew member 
	int32 selectedCharacter;

	// Number of cores in team 
	int32 cores;

	//Team color, created for testing but we might want to keep it 
	Team team;

	//Reference the map grid
	class AGrid* grid;

	//Reference to the game mode
	class ALaetusGameMode* gameMode;

	// Action Bar value, Turn will take care of creating 
	int32 actionBar; 

	/**
	* Moves the currently selected ACrewMember in the given direction
	*/
	void moveCrewMember(int32 crewMember, FVector2D direction);

	class ACrewController* controller;
};