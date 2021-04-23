// Fill out your copyright notice in the Description page of Project Settings.


#include "Rock.h"
#include "EnergizedOre.h"

static TArray<FString> rockMeshReferences = {
	"StaticMesh'/Game/Geometry/Map/HarvestSourceMeshes/ENV_Rock1.ENV_Rock1'",
	"StaticMesh'/Game/Geometry/Map/HarvestSourceMeshes/ENV_Rock2.ENV_Rock2'",
};

ARock::ARock() {
	setHarvestSourceType(HarvestSourceType::Rock);

	//Pick a random mesh
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("TreeMesh");

	int randomIdx = FMath::RandRange(0, rockMeshReferences.Num() - 1);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>randMesh1(*rockMeshReferences[0]);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>randMesh2(*rockMeshReferences[1]);
	switch (randomIdx) {
	case 0:
		mesh->SetStaticMesh(randMesh1.Object);
		break;
	default:
		mesh->SetStaticMesh(randMesh2.Object);
		break;
	}

	//mesh->AttachToComponent(mesh, FAttachmentTransformRules::KeepRelativeTransform);
	mesh->SetWorldLocation(GetActorLocation());
	SetRootComponent(mesh);
}

APowerUpActor* ARock::harvest() {
	return (APowerUpActor*) GetWorld()->SpawnActor<AEnergizedOre>(GetActorLocation(), GetActorRotation());
}

