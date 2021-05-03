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
	damage = 1;
}

/**
 * Rotate the ACrewMember in the targer direction, fire a bullet (i.e. line trace) 
 * in that cardinal direction specified in target andamages the first player it 
 * finds (if any).
 * 
 * @param direction the caridnal direction to showo the rifle in. Should be 
 *     a unit vector for one of the four cardinal directions (1,0), (0,1), (-1,0),
 *     (0,-1).
 * @return 0 if the rifle hit a player, -1 if not.
 */
int URifle::fire(FVector2D target) {
	mesh->SetVisibility(true);
	directionToShoot = target;
	
	ACrewMember* owner = Cast<ACrewMember>(GetOwner());
	owner->getCrewController()->disable();
	directionToShootEnum = owner->vectorToDirectionEnum(directionToShoot);
	float montageLength = owner->rotateWithAnimation(directionToShootEnum);
	
	if (montageLength > 0) {
		FTimerHandle timer;
		GetWorld()->GetTimerManager().SetTimer(timer, this, &URifle::shootRifle, montageLength - 0.2f, false);
	}else {
		shootRifle();
	}
	return 0;
}

/**
 * Fire the rifle in the current target direction up to range number of 
 * tiles. Stop either at max range or when the first ACrewMember gets hit.
 */
void URifle::shootRifle() {
	ACrewMember* owner = Cast<ACrewMember>(GetOwner());
	owner->rotateToDirection(directionToShootEnum);
	owner->playShootRifleMontage();
	FVector2D location = owner->getGridSpace()->getGridLocation();

	//Iterate through the tiles in this direction upto range, checking for a player.
	for (int i = 1; i <= range; i++) {
		location.X += directionToShoot.X;
		location.Y += directionToShoot.Y;

		AGridSpace* space = grid->getTile(location);
		if (space) {
			space->SetToRedOnTimer();
			AActor* occupant = space->getOccupant();
			if (occupant) {
				ACrewMember* crewMember = Cast<ACrewMember>(occupant);
				if (crewMember) {
					//TODO: determine damage to deal?
					crewMember->takeDamage(damage);
					UPowerUpEffectData* effectToApply = owner->GetWeaponEffect();
					if (effectToApply != nullptr) {
						effectToApply->ApplyCharacterEffect(crewMember);
						owner->ClearWeaponEffect();
					}
					return;
				}
			}
		}
	}

	owner->getCrewController()->enable();
	//owner->getCrewController()->setStateToIdle();
}