
// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Crew.h"
#include "CrewMember.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACrew::ACrew()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Constructor for testing 
void ACrew::SetTeam(int32 newTeam) {
	this->team = newTeam;
	
	// Set up crew members once we have team 
	SetUp();
}

// Called when the game starts or when spawned
void ACrew::BeginPlay()
{
	Super::BeginPlay();
}

//Setup Crew Members 
void ACrew::SetUp() {
	TArray<AActor*> targetPoints;

	if (team == 0) {
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATargetPoint::StaticClass(), "Red", targetPoints);
	}
	else {
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATargetPoint::StaticClass(), "Blue", targetPoints);
	}


	//Spawn  team 
	class ACrewMember* member1 = GetWorld()->SpawnActor<ACrewMember>(targetPoints[0]->GetActorLocation(), FRotator(0, 0, 0));
	class ACrewMember* member2 = GetWorld()->SpawnActor<ACrewMember>(targetPoints[1]->GetActorLocation(), FRotator(0, 0, 0));
	class ACrewMember* member3 = GetWorld()->SpawnActor<ACrewMember>(targetPoints[2]->GetActorLocation(), FRotator(0, 0, 0));

	//set team 
	/*
	if (team == 0) {
		member1->SetTeam(0);
		member2->SetTeam(0);
		member3->SetTeam(0);
	}
	else {
		member1->SetTeam(1);
		member2->SetTeam(1);
		member3->SetTeam(1);
	}
	*/

	//crewMembers.Add(member1);
	//crewMembers.Add(member2);
	//crewMembers.Add(member3);

	crewMembers[0] = member1;
	crewMembers[1] = member2;
	crewMembers[2] = member3;
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
