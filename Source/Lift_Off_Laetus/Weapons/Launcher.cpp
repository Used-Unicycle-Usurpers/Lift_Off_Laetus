// Fill out your copyright notice in the Description page of Project Settings.


#include "Launcher.h"

ULauncher::ULauncher() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>launcherMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));
	mesh->SetStaticMesh(launcherMesh.Object);
}

int ULauncher::fire(FVector2D direction) {
	mesh->SetVisibility(true);
	return -1;
}