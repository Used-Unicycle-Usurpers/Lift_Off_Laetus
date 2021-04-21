
// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Crew.h"
#include "CrewMember.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "../GameManagement/LaetusGameMode.h"
#include "../GameManagement/Grid.h"

// Sets default values
ACrew::ACrew() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//set action bar to max number 
	actionBar = 500; 

}

// Called when the game starts or when spawned
void ACrew::BeginPlay() {
	Super::BeginPlay();
}

//Setup Crew Members 
void ACrew::SetUp(int32 newTeam, AGrid* newGrid) {
	team = newTeam;
	grid = newGrid;
	TArray<int32> startingRows = grid->getStartingRows();
	
	//Set to left side, facing right
	int column = grid->getNumSteps();
	FRotator rotation = FRotator(0, 270, 0);
	if (team == 1) { // Team 1, so set to right side, facing left
		column = grid->getNumColumns() - grid->getNumSteps() - 1;
		rotation = FRotator(0, 90, 0);
	}

	//For each crew member, spawn at the next starting point and set reference to GridSpace
	for (int i = 0; i < 3; i++) {
		AGridSpace* space = grid->getTile(FVector2D(startingRows[i], column));
		FVector location = space->GetActorLocation();
		ACrewMember* newMember = GetWorld()->SpawnActor<ACrewMember>(FVector(location.X, location.Y, location.Z + 100), rotation);

		newMember->SetTeam(newTeam);
		crewMembers.Add(newMember);
		space->setOccupant(newMember);
		newMember->setGridSpace(space);
	}
}

// Return the current status of the action bar
int32 ACrew::GetActionBarStatus() {
	return actionBar;
}

// Update the action bar based on the moves performed 
void ACrew::UpdateActionBar(int32 update) {
	actionBar += update;
}

// //Return the location of the first crewMember 
FVector ACrew::GetStartingLocation() {
	return crewMembers[0]->GetActorLocation();
}

// Called every frame
void ACrew::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACrew::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
