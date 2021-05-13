// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../GameManagement/GameEnums.h"
#include "Crew.generated.h"

/**
 * A crew of ACrewMembers, representing one of the two teams in the game.
 */
UCLASS()
class LIFT_OFF_LAETUS_API ACrew : public APawn {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACrew(); 
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * Sets up this ACrew with information pertaining to the team it represents,
	 * the game grid, and the ACrewController this is going to possess this ACrew.
	 */
	void SetUp(Team newTeam, class AGrid* newGrid, class ACrewController* newController);

	/**
	 * Return the current status of the action bar
	 */
	int32 GetActionBarStatus();

	/**
	 * Update the action bar based on the moves performed.
	 */
	void UpdateActionBar(int32 update);

	/**
	 * Return the location of the first crewMember (Pavo) in world coordinates.
	 */
	FVector GetStartingLocation();

	/**
	 * Returns a reference to the currently selected ACrewMember, which is the
	 * one that is currently being affected by input (if it's this ACrew's turn).
	 */
	class ACrewMember* getCurrentCrewMember();

	/**
	 * Toggles the currently selected ACrewMember to the next one in the order.
	 * Order: Pavo (0), Lyra (1), Nemus (2)
	 */
	int toggleSelectedCrewMember();

	/**
	 * Sets the currently selected ACrewMember in this ACrew to the
	 * given integer representation of the ACrewMember.
	 */
	void setSelectedCrewMember(int current);

	/**
	 * Moves the currently selected ACrewMember in the given direction.
	 */
	void moveSelectedCrewMember(FVector2D direction);

	/**
	 * Check if we are pushing core in the given direction. Return the core if pushing one.
	 */
	class ACoreFragment* pushingCore(FVector2D direction);

	/**
	 * Sets the ACrewController that is possessiong this ACrew to the
	 * provided ACrewController.
	 */
	void setController(class ACrewController* newController);

	/**
	 * Get the integer represenation of the currently selected ACrewMember
	 * in this ACrew.
	 */
	int32 getSelectedCrewMemberIndex();

	/**
	 * Returns the number of ACoreFramgents this ACrew has collected so far.
	 */
	UFUNCTION(BlueprintCallable)
		int32 getCoreCount();

	/**
	 * Increment the number of ACoreFragments that have been collected by this ACrew
	 * by 1.
	 */
	void incrementCores();

	//The array fo crewMember that are in this ACrew.
	TArray<class ACrewMember*> crewMembers; // for testing

	/**
	 * Returns the percentage of health Pavo has remaining as a decimal.
	 */
	UFUNCTION(BlueprintCallable)
		float getPavoPercentHealth();

	/**
	 * Returns the percentage of health Lyra has remaining as a decimal.
	 */
	UFUNCTION(BlueprintCallable)
		float getLyraPercentHealth();

	/**
	 * Returns the percentage of health Nembus has remaining as a decimal.
	 */
	UFUNCTION(BlueprintCallable)
		float getNembusPercentHealth();

private:
	/**
	 * Moves the given ACrewMember (by array index) in the given direction.
	 */
	void moveCrewMember(int32 crewMember, FVector2D direction);
	
	// Index of current crew member 
	int32 selectedCharacter;

	// Number of cores in team 
	int32 cores;

	//Team color, created for testing but we might want to keep it 
	Team team;

	//Reference the map grid
	class AGrid* grid;

	// Action Bar value, Turn will take care of creating 
	int32 actionBar; 

	//The ACrewController that is possessiong this ACrew.
	class ACrewController* controller;

	class ALaetusGameMode* gameMode;
};