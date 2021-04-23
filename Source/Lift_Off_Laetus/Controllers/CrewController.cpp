// Fill out your copyright notice in the Description page of Project Settings.

#include "CrewController.h"
#include "../Characters/Crew.h"
#include "../Characters/CrewMember.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "../GameManagement/LaetusGameMode.h"

void ACrewController::SetupInputComponent() {
	Super::SetupInputComponent();
	//EnableInput(this);
	disable();
	InputComponent->BindAction("Fire", IE_Pressed, this, &ACrewController::testBinding);
	InputComponent->BindAction("ToggleCrewMember", IE_Pressed, this, &ACrewController::toggleCrewMember);
	InputComponent->BindAction("EndTurn", IE_Pressed, this, &ACrewController::endTurn);

	//Shoot rifle in one of 4 directions
	InputComponent->BindAction("ShootUp", IE_Pressed, this, &ACrewController::shootUp);
	InputComponent->BindAction("ShootLeft", IE_Pressed, this, &ACrewController::shootLeft);
	InputComponent->BindAction("ShootRight", IE_Pressed, this, &ACrewController::shootRight);
	InputComponent->BindAction("ShootDown", IE_Pressed, this, &ACrewController::shootDown);

	PlayerCameraManagerClass = PlayerCameraManager->GetClass();
}

void ACrewController::enable() {
	EnableInput(this);
}

void ACrewController::disable() {
	DisableInput(this);
}

void ACrewController::testBinding() {
	//TODO: Change to have the currently selected crew member shoot
	//ACrew* c = Cast<ACrew>(GetPawn());
	//if (c) {
	//	UE_LOG(LogTemp, Warning, TEXT("%s in testBinding, pawn is: %s!"), *GetName(), *c->GetName());
	//	c->crewMembers[0]->Shoot(FVector2D(-1, 0), false);
	//}
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

void ACrewController::endTurn() {
	ALaetusGameMode* gameMode = Cast<ALaetusGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->ChangeTurn();
}

void ACrewController::shootUp() {
	shoot(FVector2D(-1, 0));
}

void ACrewController::shootLeft() {
	shoot(FVector2D(0, -1));
}

void ACrewController::shootRight() {
	shoot(FVector2D(0, 1));
}

void ACrewController::shootDown() {
	shoot(FVector2D(1, 0));
}

void ACrewController::shoot(FVector2D direction) {
	ACrew* crew = Cast<ACrew>(GetPawn());
	crew->getCurrentCrewMember()->Shoot(direction, true);
}