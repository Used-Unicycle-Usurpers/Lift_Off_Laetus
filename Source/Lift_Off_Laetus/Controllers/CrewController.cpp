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
	ACrew* c = Cast<ACrew>(GetPawn());
	if (c) {
		UE_LOG(LogTemp, Warning, TEXT("%s in testBinding, pawn is: %s!"), *GetName(), *c->GetName());
	}
}

