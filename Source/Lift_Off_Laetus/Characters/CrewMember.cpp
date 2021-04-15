// Fill out your copyright notice in the Description page of Project Settings.


#include "CrewMember.h"
#include "Crew.h"

// Sets default values
ACrewMember::ACrewMember()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Team this character belongs to 
	//const ACrew team;

	// Character id (index in crewMember array)
	//int id;

}


// Called when the game starts or when spawned
void ACrewMember::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACrewMember::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACrewMember::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Move to GridSpace 
void ACrewMember::MoveTo(AGridSpace target) {
	// move to target grid space 
	// do we want it to return true or false to indicate 
	// if it was capable of completing the action 

	//will it decrease the crew's action bar?
}

// Shoot in a specific direction 
void ACrewMember::Shoot(FVector direction) {
	// not sure if im understanding this method correctly 
}

// Shove 
void ACrewMember::Shove() {
	// Not sure if we are going to give them the option of what to shove
}

// Take damage 
void ACrewMember::TakeDamage(int32 damage) {
	health -= damage;

	if(health <= 0)
		//destroy actor?
}
