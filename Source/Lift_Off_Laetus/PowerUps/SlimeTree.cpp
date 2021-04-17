// Fill out your copyright notice in the Description page of Project Settings.


#include "SlimeTree.h"
#include "SlimeOrb.h"

//References to random meshes to pick from on construction.
static TArray<FString> treeMeshReferences = {
	"StaticMesh'/Game/Geometry/MapPieces/HarvestSourceMeshes/ENV_Tree1.ENV_Tree1'",
	"StaticMesh'/Game/Geometry/MapPieces/HarvestSourceMeshes/ENV_Tree2.ENV_Tree2'",
	"StaticMesh'/Game/Geometry/MapPieces/HarvestSourceMeshes/ENV_Tree3.ENV_Tree3'",
};


ASlimeTree::ASlimeTree() {
	setHarvestSourceType(HarvestSourceType::SlimeTree);
	
	//Pick a random mesh
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("TreeMesh");

	int randomIdx = FMath::RandRange(0.f, (float) treeMeshReferences.Num() - 1);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>randMesh(*treeMeshReferences[randomIdx]);
	mesh->SetStaticMesh(randMesh.Object);
	mesh->AttachToComponent(mesh, FAttachmentTransformRules::KeepRelativeTransform);
	mesh->SetWorldLocation(GetActorLocation());
	SetRootComponent(mesh);
}

APowerUpActor* ASlimeTree::harvest() {
	return (APowerUpActor*) GetWorld()->SpawnActor<ASlimeOrb>(GetActorLocation(), GetActorRotation());
}