
// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Crew.h"
#include "CrewMember.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "../GameManagement/LaetusGameMode.h"
#include "../GameManagement/Grid.h"
#include "../Controllers/CrewController.h"
#include "../Controllers/InputController.h"
#include "Camera/CameraComponent.h"
#include "../Characters/CoreFragment.h"

// Sets default values
ACrew::ACrew() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bFindCameraComponentWhenViewTarget = true;

	//set action bar to max number 
	actionBar = 500;
}

// Called when the game starts or when spawned
void ACrew::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ACrew::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACrew::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

/**
 * Sets up this ACrew with information pertaining to the team it represents,
 * the game grid, and the ACrewController this is going to possess this ACrew.
 * 
 * @param newTeam the team this ACrew is on.
 * @param newGrid the game grid that will be used in this game.
 * @param newController the ACrewController that will be possessing this ACrew.
 */
void ACrew::SetUp(Team newTeam, AGrid* newGrid, ACrewController* newController) {
	team = newTeam;
	grid = newGrid;
	setController(newController);
	TArray<int32> startingRows = grid->getStartingRows();
	
	//Set to left side, facing right
	int column = grid->getNumSteps();
	FRotator rotation = FRotator(0, 270, 0);
	if (team == Team::Blue) { // Team 1, so set to right side, facing left
		column = grid->getNumColumns() - grid->getNumSteps() - 1;
		rotation = FRotator(0, 90, 0);
	}

	//For each crew member, spawn at the next starting point and set reference to GridSpace
	for (int i = 0; i < 3; i++) {
		AGridSpace* space = grid->getTile(FVector2D(startingRows[i], column));
		FVector location = space->GetActorLocation();
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ACrewMember* newMember = GetWorld()->SpawnActor<ACrewMember>(location + FVector(0.f, 0.f, 20.f), rotation, params);
		
		newMember->setMeshAnimData((FCharacter) i, newTeam);
		newMember->setCrewController(controller);
		newMember->SetTeam(newTeam);
		crewMembers.Add(newMember);
		newMember->setGridSpace(space);
	}
}

/**
 * Return the current status of the action bar
 * 
 * @return an integer between 0-10, representing the amount of
 *     action points this ACrew has left this turn.
 */
int32 ACrew::GetActionBarStatus() {
	return actionBar;
}

/**
 * Update the action bar based on the moves performed.
 * 
 * @param update the number of action points to update the action bar by.
 *     A negative value will subtract action points, a positive value will
 *     add them.
 */ 
void ACrew::UpdateActionBar(int32 update) {
	actionBar += update;
}

/**
 * Return the location of the first crewMember (Pavo) in world coordinates.
 * 
 * @return the location of the first crewMember (Pavo) in world coordinates.
 */
FVector ACrew::GetStartingLocation() {
	return crewMembers[0]->GetActorLocation();
}

/**
 * Returns a reference to the currently selected ACrewMember, which is the
 * one that is currently being affected by input (if it's this ACrew's turn).
 * 
 * @return a pointer to the ACrewMember that is curretnly selected in
 *     this ACrew.
 */
ACrewMember* ACrew::getCurrentCrewMember() {
	return crewMembers[selectedCharacter];
}

/**
 * Toggles the currently selected ACrewMember to the next one in the order.
 * Order: Pavo (0), Lyra (1), Nemus (2)
 * 
 * @return the integer representation of the newly selected ACrewMember
 *     in this ACrew.
 */
int ACrew::toggleSelectedCrewMember() {
	selectedCharacter++;
	if (selectedCharacter > 2) {
		selectedCharacter = 0;
	}
	return selectedCharacter;
}

/**
 * Sets the currently selected ACrewMember in this ACrew to the
 * given integer representation of the ACrewMember.
 * 
 * @param current the integer represntion of the ACrewMemebr to choose
 *     as the currently selected ACrewMember in this ACrew. 0 = Pavo,
 *     1 = Lyra, 2 = Nembus.
 */
void ACrew::setSelectedCrewMember(int current) {
	selectedCharacter = current;
	controller->getInputController()->moveCameraToCrewMember();
}

/**
* Moves the given ACrewMember (by array index) in the given direction.
* 
* @param crewMemberID the integer represention of the ACrewMember being
*     moved. 0 = Pavo, 1 = Lyra, 2 = Nembus.
* @param direction a unit vector represtenting the carindal direction to
*     move the ACrewMember specified by crewMemberID.
*/
void ACrew::moveCrewMember(int32 crewMemberID, FVector2D direction) {
	if (crewMemberID >= crewMembers.Num()) { return; }

	FVector2D crewMemberGridLocation = crewMembers[selectedCharacter]->getGridSpace()->getGridLocation();
	AGridSpace* destination = grid->getTile(crewMemberGridLocation + direction);

	if (destination && !(destination->isOccupied())) {
		crewMembers[selectedCharacter]->MoveTo(destination, false);
	}else {

		if (destination && destination->containsFragment()) {

			//Check where the fragment will go
			AGridSpace* fragmentDest = grid->getTile(crewMemberGridLocation + (2 * direction));
			if (fragmentDest && !fragmentDest->isOccupied()) {
				
				//Can move, push core fragment first
				ACoreFragment* fragment = Cast<ACoreFragment>(destination->getOccupant());
				if (fragment) {
					//Move the core fragment first so that space is no longer occupired and thus
					//the crew member can move as well.
					fragment->moveTo(fragmentDest, crewMembers[selectedCharacter]);
					crewMembers[selectedCharacter]->MoveTo(destination, true);
				}
			}
		}
	}
}

/**
* Moves the currently selected ACrewMember in the given direction.
* 
* @param direction a unit vector represtenting the carindal direction to
*     move the currently selected ACrewMember in this ACrew.
*/
void ACrew::moveSelectedCrewMember(FVector2D direction) {
	moveCrewMember(selectedCharacter, direction);
}

/**
* Check if we are pushing core in the given direction.
* 
* @param directoin the unit vector representing the carindal direction
*     in which we are checking if we're pushing an ACoreFragment.
*/
bool ACrew::pushingCore(FVector2D direction) {
	FVector2D crewMemberGridLocation = crewMembers[selectedCharacter]->getGridSpace()->getGridLocation();
	AGridSpace * destination = grid->getTile(crewMemberGridLocation + direction);
	if (destination && destination->containsFragment()) { return true; }
	
	return false;
}

/**
 * Sets the ACrewController that is possessiong this ACrew to the 
 * provided ACrewController.
 * 
 * @param newController the ACrewController that is now possessing
 *     this ACrew.
 */
void ACrew::setController(ACrewController* newController) {
	controller = newController;
}

/**
 * Get the integer represenation of the currently selected ACrewMember 
 * in this ACrew.
 * 
 * @return the integer represtion of the currently selected ACrewMember 
 *     in this ACrew. 0 = Pavo, 1 = Lyra, 2 = Nembus.
 */
int32 ACrew::getSelectedCrewMemberIndex() {
	return selectedCharacter;
}

/**
 * Returns the number of ACoreFramgents this ACrew has collected so far.
 * 
 * @return an integer representing the number of ACoreFragments this ACrew has
 *     collected so far.
 */
int32 ACrew::getCoreCount() {
	return cores;
}

/**
 * Increment the number of ACoreFragments that have been collected by this ACrew
 * by 1.
 */
void ACrew::incrementCores() {
	cores += 1;
}