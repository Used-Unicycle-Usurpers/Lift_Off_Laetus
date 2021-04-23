// Fill out your copyright notice in the Description page of Project Settings.

#include "CrewController.h"
#include "../Characters/Crew.h"
#include "../Characters/CrewMember.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

void ACrewController::SetupInputComponent() {
	Super::SetupInputComponent();
	EnableInput(this);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ACrewController::testBinding);
	InputComponent->BindAction("ToggleCrewMember", IE_Pressed, this, &ACrewController::toggleCrewMember);
	PlayerCameraManagerClass = PlayerCameraManager->GetClass();
}

void ACrewController::testBinding() {
	//TODO: Change to have the currently selected crew member shoot
	ACrew* c = Cast<ACrew>(GetPawn());
	if (c) {
		UE_LOG(LogTemp, Warning, TEXT("%s in testBinding, pawn is: %s!"), *GetName(), *c->GetName());
		c->crewMembers[0]->Shoot(FVector2D(-1, 0), false);
	}
}

/**
 * Sets up the APlayerCameraManager reference so all controllers affect the 
 * same camera manager.
 */
void ACrewController::initCamera() {
	cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
}

/**
 * Moves the camera to the current ACrewMember.
 */
void ACrewController::moveCameraToCrewMember() {
	ACrew* c = Cast<ACrew>(GetPawn());
	if (c) {
		ACrewMember* current = c->getCurrentCrewMember();
		FViewTargetTransitionParams p;
		p.BlendFunction = EViewTargetBlendFunction::VTBlend_Linear;
		p.BlendTime = 1.f;
		cameraManager->SetViewTarget(current, p);
	}
}

/**
 * Toggle the currently selected crew member and focus the camera 
 * on them
 */
void ACrewController::toggleCrewMember() {
	ACrew* c = Cast<ACrew>(GetPawn());
	c->toggleSelectedCrewMember();
	moveCameraToCrewMember();
}

