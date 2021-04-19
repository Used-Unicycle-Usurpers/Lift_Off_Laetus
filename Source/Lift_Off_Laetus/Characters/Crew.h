// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Crew.generated.h"

UCLASS()
class LIFT_OFF_LAETUS_API ACrew : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACrew(); 
	
	//for testing 
	void SetTeam(int32 team);
	void SetUp();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Array of crew members 
	//class ACrewMember* crewMembers[3] = {};
	TArray<AActor*> crewMembers; // for testing

	// Index of current crew member 
	int32 selectedCharacter;

	// Number of cores in team 
	int32 cores;

	//Team color, created for testing but we might want to keep it 
	int32 team; 


	// Would the ActionBar component go here? 
};