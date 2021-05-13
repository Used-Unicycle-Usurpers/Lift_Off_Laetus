// Fill out your copyright notice in the Description page of Project Settings.


#include "Launcher.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/CrewMember.h"
#include "../GameManagement/GridSpace.h"
#include "../GameManagement/Grid.h"
#include "Grenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../Controllers/CrewController.h"
#include "../Controllers/InputController.h"
#include "Sound/SoundCue.h"

ULauncher::ULauncher() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>launcherMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/grenade.grenade'"));
	mesh->SetStaticMesh(launcherMesh.Object);
	mesh->SetWorldScale3D(FVector(25.f, 25.f, 25.f));
	range = 3;

	static ConstructorHelpers::FObjectFinder<USoundCue>sound(TEXT("SoundCue'/Game/Audio/Weapons/AUD_launcher02_Cue.AUD_launcher02_Cue'"));
	launcherSound = sound.Object;
}

/**
 * Throws a grenade at the AGridSpace specified in direction.
 * 
 * @param direction a 2D vector in which the X component is the 
 *     row of the target space, and the Y component is the column 
 *     of the desired space.
 */
int ULauncher::fire(FVector2D target) {
	ACrewMember* owner = Cast<ACrewMember>(GetOwner());
	owner->getCrewController()->disableInputController();
	directionToFaceEnum = getDirectionToThrow(target);
	float montageLength = owner->rotateWithAnimation(directionToFaceEnum);
	targetSpace = target;
	grid->clearGridOverlay();

	if (montageLength > 0) {
		FTimerHandle timerParams;
		GetWorld()->GetTimerManager().SetTimer(timerParams, this, &ULauncher::readyLaunch, montageLength - 0.2f, false);
	}else {
		readyLaunch();
	}
	return 0;
}

/**
 * Prepare to throw the grenade by starting the throw animation and setting a timer
 * to later actually throw it.
 */
void ULauncher::readyLaunch() {
	//Finshed rotating montage, so actually set the rotation of the actor
	ACrewMember* owner = Cast<ACrewMember>(GetOwner());
	owner->rotateToDirection(directionToFaceEnum);

	//About to throw the grenade, so show it in the player's hand.
	mesh->SetVisibility(true);
	
	owner->playThrowMontage();
	FTimerHandle timerParams;
	GetWorld()->GetTimerManager().SetTimer(timerParams, this, &ULauncher::launch, owner->throwMontageDelay, false);
}

/**
 * Caluclate a perfect arc from the player's location the target AGridSpace, 
 * and throw the grenade.
 */
void ULauncher::launch() {
	UGameplayStatics::PlaySound2D(GetWorld(), launcherSound);
	ACrewMember* owner = Cast<ACrewMember>(GetOwner());
	FVector2D location = owner->getGridSpace()->getGridLocation();
	AGridSpace* space = grid->getTile(targetSpace);

	if (space) {
		//space->SetToRed();
		FVector start = mesh->GetComponentLocation();
		FVector end = space->GetActorLocation();

		//Get the intial velocity that would be needed for the toss
		FVector velocity = FVector(0, 0, 0);
		UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), velocity, start, end, 0.f, 0.3f);

		//Get a path of points that make up the arc
		FPredictProjectilePathParams p;
		p.StartLocation = start;
		p.LaunchVelocity = velocity;
		p.SimFrequency = 15;
		p.MaxSimTime = 20.f;
		p.bTraceWithCollision = true;
		p.bTraceWithChannel = true;
		p.TraceChannel = ECollisionChannel::ECC_GameTraceChannel1;
		p.ActorsToIgnore.Add(GetOwner());
		FPredictProjectilePathResult r;
		UGameplayStatics::PredictProjectilePath(GetWorld(), p, r);

		//Spawn the grenade and pass the path for it to traverse.
		grid->clearGridOverlay();
		space->SetOverlayToRed(false);
		AGrenade* g = GetWorld()->SpawnActor<AGrenade>(mesh->GetComponentLocation(), FRotator(0, 0, 0));
		g->path = r.PathData;
		g->targetLocation = end;
		g->targetSpace = space;
		g->grid = grid;
		g->owner = owner;

		//Now have the camera follow the grenade as it flies through the air.
		ACrewController* controller = owner->getCrewController();
		controller->getInputController()->moveCameraSmoothly(g);
	}

	//Hide the grenade launcher, since we have now finished 
	//throwing a grenade.
	mesh->SetVisibility(false);
}

/**
 * Based on the coordinates of the given target AGridSpace, calculate which 
 * of the four caridinal directions to rotate to that most closely matches the 
 * direction you're throwing the grenade to. This is useful when throwing a
 * grenade diagonally, so you can get which of the four directions most closely 
 * matches that direction to face.
 * 
 * @param target a 2D vector that contains the (row, column) coordinates of the 
 *     AGridSpace that a grenade is about to be thrown to.
 * @return a Direction enum for one of the four cardinal directions that most closely
 *     matches the direction the ACrewMember should be facing when throwing the grenade.
 */
Direction ULauncher::getDirectionToThrow(FVector2D target) {
	ACrewMember* owner = Cast<ACrewMember>(GetOwner());
	FVector2D current = target - owner->getGridSpace()->getGridLocation();
	current.Normalize();

	//Calucate a dot product against all four carindal directions.
	TArray<float> dotProducts;
	dotProducts.Add(FVector2D::DotProduct(current, FVector2D(-1, 0)));//Up
	dotProducts.Add(FVector2D::DotProduct(current, FVector2D(0, -1)));//Left
	dotProducts.Add(FVector2D::DotProduct(current, FVector2D(0, 1)));//Right
	dotProducts.Add(FVector2D::DotProduct(current, FVector2D(1, 0)));//Down
	
	//The max dot product will be with the vector that this trajveoty most closely 
	//matches and thus is the direction the ACrewMember should face when throwing 
	//the grenade.
	int32 maxIdx;
	FMath::Max(dotProducts, &maxIdx);

	//Return the corresponding Direction enum
	switch (maxIdx) {
	case 0:
		return Direction::Up;
	case 1:
		return Direction::Left;
	case 2:
		return Direction::Right;
	case 3:
		return Direction::Down;
	default:
		return Direction::InvalidDirection;
	}
}