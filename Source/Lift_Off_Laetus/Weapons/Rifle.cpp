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

/**
 * Fires a bullet (i.e. line trace) in the cardinal direction specified in 
 * direction and damages the first player it finds (if any).
 * 
 * @param direction the caridnal direction to showo the rifle in. Should be 
 *     a unit vector for one of the four cardinal directions (1,0), (0,1), (-1,0),
 *     (0,-1).
 * @return 0 if the rifle hit a player, -1 if not.
 */
int URifle::fire(FVector2D direction) {
	mesh->SetVisibility(true);

	ACrewMember* owner = Cast<ACrewMember>(GetOwner());
	FVector2D location = owner->getGridSpace()->getGridLocation();
	UE_LOG(LogTemp, Warning, TEXT("Current location: (%f,%f)"), location.X, location.Y);

	//Iterate through the tiles in this direction upto range, checking for a player.
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
				return 0;
			}
		}
	}

	return -1;
}