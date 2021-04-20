
// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Crew.h"
#include "CrewMember.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "../GameManagement/LaetusGameMode.h"
#include "../GameManagement/Grid.h"

// Sets default values
ACrew::ACrew()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

/*
// Constructor for testing 
void ACrew::SetTeam(int32 newTeam) {
	team = newTeam;
	
	// Set up crew members once we have team 
	//SetUp();
}
*/

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

// Called every frame
void ACrew::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACrew::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Character selection
	PlayerInputComponent->BindAction("Action_CycleCrewUp", IE_Pressed, this, &ACrew::CycleSelectedCrewMemberUp);
	PlayerInputComponent->BindAction("Action_CycleCrewDown", IE_Pressed, this, &ACrew::CycleSelectedCrewMemberDown);

	// Movement
	// TODO: Remove these! They're just for testing
	PlayerInputComponent->BindAction("DPad_Up", IE_Pressed, this, &ACrew::MoveCurrentUp);
	PlayerInputComponent->BindAction("DPad_Down", IE_Pressed, this, &ACrew::MoveCurrentDown);
	PlayerInputComponent->BindAction("DPad_Right", IE_Pressed, this, &ACrew::MoveCurrentRight);
	PlayerInputComponent->BindAction("DPad_Left", IE_Pressed, this, &ACrew::MoveCurrentLeft);
}

// Movement

void ACrew::MoveCrewMember(int32 crewMember, FVector2D direction) {
	
	if (crewMember >= crewMembers.Num()) { return; }
	
	AGridSpace* adjSpace = grid->getTileRelative(direction, crewMembers[crewMember]->getGridSpace());
	
	if (adjSpace != nullptr && !(adjSpace->isOccupied())) {
		crewMembers[crewMember]->MoveTo(adjSpace);
	}
}

void ACrew::MoveCurrentRight() {
	MoveCrewMember(selectedCharacter, FVector2D(1, 0));
}

void ACrew::MoveCurrentLeft() {
	MoveCrewMember(selectedCharacter, FVector2D(-1, 0));
}

void ACrew::MoveCurrentUp() {
	MoveCrewMember(selectedCharacter, FVector2D(0, 1));
}

void ACrew::MoveCurrentDown() {
	MoveCrewMember(selectedCharacter, FVector2D(0, -1));
}

// Crew member selection

void ACrew::SelectCrewMember(int32 selected) {
	if (selected >= crewMembers.Num() || selected == selectedCharacter) { return; }
	
	selectedCharacter = selected;
	// Refresh UI here, maybe?
}

void ACrew::CycleSelectedCrewMemberUp() {
	
	if (selectedCharacter == crewMembers.Num() - 1)
		SelectCrewMember(0);
	else
		SelectCrewMember(selectedCharacter + 1);
}

void ACrew::CycleSelectedCrewMemberDown() {

	if (selectedCharacter == 0)
		SelectCrewMember(crewMembers.Num() - 1);
	else
		SelectCrewMember(selectedCharacter - 1);
}
