// Fill out your copyright notice in the Description page of Project Settings.

#include "CrewController.h"
#include "../Characters/Crew.h"
#include "../Characters/CrewMember.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "../GameManagement/LaetusGameMode.h"

ACrewController::ACrewController() {

}

void ACrewController::SetupInputComponent() {
	Super::SetupInputComponent();
	//EnableInput(this);
	disable();
	InputComponent->BindAction("ToggleCrewMember", IE_Pressed, this, &ACrewController::toggleCrewMember);
	InputComponent->BindAction("EndTurn", IE_Pressed, this, &ACrewController::endTurn);

	//Shoot rifle in one of 4 directions
	InputComponent->BindAction("ShootUp", IE_Pressed, this, &ACrewController::shootUp);
	InputComponent->BindAction("ShootLeft", IE_Pressed, this, &ACrewController::shootLeft);
	InputComponent->BindAction("ShootRight", IE_Pressed, this, &ACrewController::shootRight);
	InputComponent->BindAction("ShootDown", IE_Pressed, this, &ACrewController::shootDown);

	//Launch a grenade in one of 4 directions
	InputComponent->BindAction("LaunchUp", IE_Pressed, this, &ACrewController::launchUp);
	InputComponent->BindAction("LaunchLeft", IE_Pressed, this, &ACrewController::launchLeft);
	InputComponent->BindAction("LaunchRight", IE_Pressed, this, &ACrewController::launchRight);
	InputComponent->BindAction("LaunchDown", IE_Pressed, this, &ACrewController::launchDown);

	PlayerCameraManagerClass = PlayerCameraManager->GetClass();
}

void ACrewController::enable() {
	EnableInput(this);
}

void ACrewController::disable() {
	DisableInput(this);
}

/**
 * Sets up the APlayerCameraManager reference so all controllers affect the 
 * same camera manager.
 */
void ACrewController::init() {
	cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	/*
	TArray<UUserWidget*> results;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), results, HUDWidgetClass);
	if (results[0]) {
		hud = results[0];
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find hud in world"));
	}
	*/
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
	if (crew) {
		crew->getCurrentCrewMember()->Shoot(direction, true);
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Tried to shoot rifle, but controlled Crew pawn was null for controller %s"), *GetName());
	}
}

void ACrewController::launchUp() {
	launch(FVector2D(-1, 0));
}

void ACrewController::launchLeft() {
	launch(FVector2D(0, -1));
}

void ACrewController::launchRight() {
	launch(FVector2D(0, 1));
}

void ACrewController::launchDown() {
	launch(FVector2D(1, 0));
}

void ACrewController::launch(FVector2D direction) {
	ACrew* crew = Cast<ACrew>(GetPawn());
	if (crew) {
		crew->getCurrentCrewMember()->Shoot(direction, false);
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Tried to launch a grenade, but controlled Crew pawn was null for controller %s"), *GetName());
	}
}