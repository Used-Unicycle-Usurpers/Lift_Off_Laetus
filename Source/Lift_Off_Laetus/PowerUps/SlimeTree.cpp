// Fill out your copyright notice in the Description page of Project Settings.


#include "SlimeTree.h"
#include "SlimeOrb.h"

//References to random meshes to pick from on construction.
static TArray<FString> treeTrunkReferences = {
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__TreeBase.sampleMap_with_Environment__1__TreeBase'",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__TreeBase1.sampleMap_with_Environment__1__TreeBase1",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__TreeBase2.sampleMap_with_Environment__1__TreeBase2'",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__TreeBase3.sampleMap_with_Environment__1__TreeBase3'",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__TreeBase4.sampleMap_with_Environment__1__TreeBase4'",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__TreeBase5.sampleMap_with_Environment__1__TreeBase5'",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__TreeBase6.sampleMap_with_Environment__1__TreeBase6'",
};

static TArray<FString> treeBushReferences = {
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__Bush1.sampleMap_with_Environment__1__Bush1'",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__Bush111.sampleMap_with_Environment__1__Bush111'",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__Bush1116.sampleMap_with_Environment__1__Bush1116'",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__Bush1119.sampleMap_with_Environment__1__Bush1119'",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__Bush1122.sampleMap_with_Environment__1__Bush1122'",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__Bush1125.sampleMap_with_Environment__1__Bush1125'",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__Bush18.sampleMap_with_Environment__1__Bush18'",
};

ASlimeTree::ASlimeTree() {
	setHarvestSourceType(HarvestSourceType::SlimeTree);
	/*
	//Pick a random trunk and brush mesh
	trunkMesh = CreateDefaultSubobject<UStaticMeshComponent>("TreeMesh");
	bushMesh = CreateDefaultSubobject<UStaticMeshComponent>("BushMesh");

	int randomTrunk = FMath::RandRange(0.f, (float) treeTrunkReferences.Num() - 1);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>randTrunk(*treeTrunkReferences[randomTrunk]);
	trunkMesh->SetStaticMesh(randTrunk.Object);
	trunkMesh->AttachToComponent(mesh, FAttachmentTransformRules::KeepRelativeTransform);
	//trunkMesh->SetWorldLocation(GetActorLocation());
	//SetRootComponent(trunkMesh);

	int randomBush = FMath::RandRange(0.f, (float) treeBushReferences.Num() - 1);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>randBush(*treeBushReferences[randomBush]);
	bushMesh->SetStaticMesh(randBush.Object);
	bushMesh->AttachToComponent(trunkMesh, FAttachmentTransformRules::KeepRelativeTransform);
	/*
	for (int i = 0; i < treeTrunkReferences.Num(); i++) {
		static ConstructorHelpers::FObjectFinder<UStaticMesh>temp(*treeTrunkReferences[i]);
		treeTrunks.Add(temp.Object);
	}
	*/
}

APowerUpActor* ASlimeTree::harvest() {
	return (APowerUpActor*) GetWorld()->SpawnActor<ASlimeOrb>(GetActorLocation(), GetActorRotation());
}