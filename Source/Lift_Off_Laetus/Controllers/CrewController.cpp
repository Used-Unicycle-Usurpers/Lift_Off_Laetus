// Fill out your copyright notice in the Description page of Project Settings.


#include "CrewController.h"
#include "../Characters/Crew.h"
#include "../Characters/CrewMember.h"

void ACrewController::SetupInputComponent() {
	Super::SetupInputComponent();
	EnableInput(this);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ACrewController::testBinding);
}

void ACrewController::testBinding() {
	//TODO: Change to have the currently selected crew member shoot
	ACrew* c = Cast<ACrew>(GetPawn());
	if (c) {
		UE_LOG(LogTemp, Warning, TEXT("%s in testBinding, pawn is: %s!"), *GetName(), *c->GetName());
		c->crewMembers[0]->Shoot(FVector2D(-1, 0), false);
	}
}

