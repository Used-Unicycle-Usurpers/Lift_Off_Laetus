// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Crew.generated.h"

UCLASS()
class LIFT_OFF_LAETUS_API ACrew : public APawn {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACrew(); 
	
	//void SetTeam(int32 newTeam);
	void SetUp(int32 newTeam, class AGrid* newGrid);

	//Action bar stuff 
	int32 GetActionBarStatus();
	void UpdateActionBar(int32 update);

	//Return the location of the first crewMember 
	FVector GetStartingLocation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * Returns a reference to the currently selected ACrewMember.
	 */
	ACrewMember* getCurrentCrewMember();

private:
	// Array of crew members 
	TArray<class ACrewMember*> crewMembers;

	// Index of current crew member 
	int32 selectedCharacter;

	// Number of cores in team 
	int32 cores;

	//Team color, created for testing but we might want to keep it 
	int32 team;

	//Reference the map grid
	class AGrid* grid;

	//Reference to the game mode
	class ALaetusGameMode* gameMode;
	// Action Bar value, Turn will take care of creating 
	int32 actionBar; 

	///////////////
	// Functions //
	///////////////
	
	// Tries to move the given crew member in the given direction
	void MoveCrewMember(int32 crewMember, FVector2D direction);

	// Movement methods in the cardinal directions for the current player
	void MoveCurrentLeft();
	void MoveCurrentRight();
	void MoveCurrentUp();
	void MoveCurrentDown();

	// Select a crew member
	void SelectCrewMember(int32 selected);

	// Cycle the crew member selection up (right button) or down (left)
	void CycleSelectedCrewMemberUp();
	void CycleSelectedCrewMemberDown();

};
