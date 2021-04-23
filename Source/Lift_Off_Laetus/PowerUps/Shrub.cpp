// Fill out your copyright notice in the Description page of Project Settings.


#include "Shrub.h"
#include "ShrubFruit.h"

static TArray<FString> shrubMeshReferences = {
	"StaticMesh'/Game/Geometry/Map/HarvestSourceMeshes/ENV_Shrub.ENV_Shrub'",
};

AShrub::AShrub() {
	setHarvestSourceType(HarvestSourceType::Shrub);

	//Pick a random mesh
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("TreeMesh");

	int randomIdx = FMath::RandRange(0, shrubMeshReferences.Num() - 1);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>randMesh1(*shrubMeshReferences[0]);
	
	//Using a switch here so we can easily add more shrub meshes later.
	switch (randomIdx) {
	case 0:
	default:
		mesh->SetStaticMesh(randMesh1.Object);
	}

	//mesh->AttachToComponent(mesh, FAttachmentTransformRules::KeepRelativeTransform);
	mesh->SetWorldLocation(GetActorLocation());
	SetRootComponent(mesh);
}

APowerUpActor* AShrub::harvest() {
	return (APowerUpActor*) GetWorld()->SpawnActor<AShrubFruit>(GetActorLocation(), GetActorRotation());
}