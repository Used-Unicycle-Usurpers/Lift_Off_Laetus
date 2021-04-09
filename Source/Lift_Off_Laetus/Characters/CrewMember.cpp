// Fill out your copyright notice in the Description page of Project Settings.


#include "CrewMember.h"

// Sets default values
ACrewMember::ACrewMember()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

