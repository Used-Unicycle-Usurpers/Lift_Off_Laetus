// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

URifle::URifle() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>rifleMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));
	mesh->SetStaticMesh(rifleMesh.Object);
}

int URifle::fire(FVector2D direction) {
	mesh->SetVisibility(true);
	return -1;
}