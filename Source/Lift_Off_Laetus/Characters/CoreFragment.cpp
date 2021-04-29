// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreFragment.h"
#include "../GameManagement/GridSpace.h"
#include "../GameManagement/CoreFragmentReceiver.h"

// Sets default values
ACoreFragment::ACoreFragment() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	static ConstructorHelpers::FObjectFinder<UStaticMesh>CubeMeshAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/TEAM_CoreFragment__2_.TEAM_CoreFragment__2_'"));
	mesh->SetStaticMesh(CubeMeshAsset.Object);
	//mesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
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

	// Actually move actor to the position
	FVector newLocation = target->GetActorLocation();
	SetActorLocation(newLocation);

	// Check if the core fragment has been pushed into a receiver
	if (target->getGridLocation().X == 0) {
		
	}
}

void ACoreFragment::setGridSpace(AGridSpace* space) {
	if (space && !space->isOccupied()) {

		if (gridSpace) {
			gridSpace->setOccupant(nullptr);
		}

		space->setOccupant(this);
		gridSpace = space;

		ACoreFragmentReceiver* receiver = Cast<ACoreFragmentReceiver>(space);

		if (receiver != nullptr) {
			receiver->OnCoreFragmentReceived(this);
		}
	}
}

AGridSpace* ACoreFragment::getGridSpace() {
	return gridSpace;
}