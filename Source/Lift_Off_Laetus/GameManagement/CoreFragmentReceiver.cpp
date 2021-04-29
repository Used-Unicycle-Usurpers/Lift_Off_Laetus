// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreFragmentReceiver.h"
#include "Lift_Off_Laetus/Characters/Crew.h"
#include "Lift_Off_Laetus/Characters/CoreFragment.h"

void ACoreFragmentReceiver::BeginPlay() {
	Super::BeginPlay();

	// TODO: Replace this scaling with something better
	mesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 0.1f));
}

void ACoreFragmentReceiver::SetCrew(ACrew* crew) {
	ownedBy = crew;
}

void ACoreFragmentReceiver::OnCoreFragmentReceived(ACoreFragment* core) {

	// Increase core count by 1
	ownedBy->incrementCores();

	// Destroy the 
	GetWorld()->DestroyActor(core);
}