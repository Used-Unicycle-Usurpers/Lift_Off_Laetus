// Fill out your copyright notice in the Description page of Project Settings.


#include "CrewController.h"
#include "../Characters/Crew.h"
#include "../Characters/CrewMember.h"

void ACrewController::SetupInputComponent() {
	Super::SetupInputComponent();
	EnableInput(this);
	InputComponent->BindKey(EKeys::AnyKey, IE_Pressed, this, &ACrewController::testBinding);
}

void ACrewController::testBinding() {
	ACrew* crew = Cast<ACrew>(GetPawn());
	if (crew) {
		ACrewMember* crewMember = crew->crewMembers[0];
		crewMember->Shoot(FVector2D(0, 0), true);
	}
}

