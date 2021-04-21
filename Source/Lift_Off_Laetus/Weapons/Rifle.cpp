// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"
#include "../Characters/CrewMember.h"
#include "../Characters/Crew.h"
#include "../Controllers/CrewController.h"
#include "../GameManagement/GridSpace.h"
#include "../GameManagement/LaetusGameMode.h"
#include "../GameManagement/Grid.h"
#include "Kismet/GameplayStatics.h"

URifle::URifle() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>rifleMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/gunMockup.gunMockup'"));
	mesh->SetStaticMesh(rifleMesh.Object);
	mesh->SetWorldScale3D(FVector(10.f, 10.f, 10.f));

	range = 5;
	damage = 5;
}

int URifle::fire(FVector2D direction) {
	mesh->SetVisibility(true);

	ACrewMember* owner = Cast<ACrewMember>(GetOwner());
	FVector2D location = owner->getGridSpace()->getGridLocation();
	UE_LOG(LogTemp, Warning, TEXT("Current location: (%f,%f)"), location.X, location.Y);

	for (int i = 1; i <= range; i++) {
		location.X += direction.X;
		location.Y += direction.Y;
		UE_LOG(LogTemp, Warning, TEXT("Now checking: (%f,%f)"), location.X, location.Y);

		AGridSpace* space = grid->getTile(location);
		if (space) {
			space->SetToRed();
			ACrewMember* occupant = space->getOccupant();
			if (occupant) {
				UE_LOG(LogTemp, Warning, TEXT("Hit %s!"), *occupant->GetName());
				//TODO: determine damage to deal?
				occupant->takeDamage(damage);
			}
		}
	}

	return -1;
}