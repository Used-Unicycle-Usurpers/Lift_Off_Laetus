// Fill out your copyright notice in the Description page of Project Settings.


#include "Crew.h"
#include "CrewMember.h"

// Sets default values
ACrew::ACrew()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACrew::BeginPlay()
{
	Super::BeginPlay();
	
	// Array of crew members 
	ACrewMember crewMembers[5] = {};
	
	// Index of current crew member 
	int selectedCharacter =  0;

	// Number of cores in team 
	int cores = 0;
}

// Called every frame
void ACrew::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACrew::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

