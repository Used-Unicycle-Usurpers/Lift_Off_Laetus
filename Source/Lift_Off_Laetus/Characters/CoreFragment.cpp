// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreFragment.h"
#include "../GameManagement/GridSpace.h"

// Sets default values
ACoreFragment::ACoreFragment() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	static ConstructorHelpers::FObjectFinder<UStaticMesh>CubeMeshAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));
	mesh->SetStaticMesh(CubeMeshAsset.Object);
	mesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
}

// Called when the game starts or when spawned
void ACoreFragment::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void ACoreFragment::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ACoreFragment::moveTo(AGridSpace* target) {
	if (target == nullptr || target->isOccupied()) {
		return;
	}

	// Reset pointers/references
	setGridSpace(target);

	FVector newLocation = target->GetActorLocation();
	SetActorLocation(newLocation);
}

void ACoreFragment::setGridSpace(AGridSpace* space) {
	if (space && !space->isOccupied()) {

		if (gridSpace) {
			gridSpace->setOccupant(nullptr);
		}

		space->setOccupant(this);
		gridSpace = space;
	}
}

AGridSpace* ACoreFragment::getGridSpace() {
	return gridSpace;
}