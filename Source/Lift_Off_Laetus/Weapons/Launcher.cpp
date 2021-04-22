// Fill out your copyright notice in the Description page of Project Settings.


#include "Launcher.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/CrewMember.h"
#include "../GameManagement/GridSpace.h"
#include "../GameManagement/Grid.h"
#include "Grenade.h"
#include "GameFramework/ProjectileMovementComponent.h"

ULauncher::ULauncher() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>launcherMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/grenade.grenade'"));
	mesh->SetStaticMesh(launcherMesh.Object);
	mesh->SetWorldScale3D(FVector(25.f, 25.f, 25.f));
}

/**
 * Throws a grenade at the AGridSpace specified in direction.
 * 
 * @param direction a 2D vector in which the X component is the 
 *     row of the target space, and the Y component is the column 
 *     of the desired space.
 */
int ULauncher::fire(FVector2D direction) {
	ACrewMember* owner = Cast<ACrewMember>(GetOwner());
	FVector2D location = owner->getGridSpace()->getGridLocation();
	AGridSpace* space = grid->getTile(FVector2D(location.X, location.Y - 6));
	if (space) {
		targetDirection = direction;
		mesh->SetVisibility(true);
		owner->playThrowMontage();
		FTimerHandle timerParams;
		GetWorld()->GetTimerManager().SetTimer(timerParams, this, &ULauncher::launch, 1.8f, false);
		return 0;
	}
	return -1;
}

void ULauncher::launch() {
	ACrewMember* owner = Cast<ACrewMember>(GetOwner());
	FVector2D location = owner->getGridSpace()->getGridLocation();
	AGridSpace* space = grid->getTile(FVector2D(location.X, location.Y - 10));
	space->SetToRed();

	if (space) {
		FVector start = mesh->GetComponentLocation();
		FVector end = space->GetActorLocation();

		//Get the intial velocity that would be needed for the toss
		FVector velocity = FVector(0, 0, 0);
		UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), velocity, start, end, 0.f, 0.3f);

		//Get a path of points that make up the arc
		FPredictProjectilePathParams p;
		p.StartLocation = start;
		p.LaunchVelocity = velocity;
		p.DrawDebugType = EDrawDebugTrace::Persistent;
		p.DrawDebugTime = 100.f;
		p.SimFrequency = 15;
		p.MaxSimTime = 20.f;
		p.bTraceWithCollision = true;
		p.ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
		p.ActorsToIgnore.Add(GetOwner());
		FPredictProjectilePathResult r;
		UGameplayStatics::PredictProjectilePath(GetWorld(), p, r);

		//Spawn the grenade and pass the path for it to traverse.
		AGrenade* g = GetWorld()->SpawnActor<AGrenade>(mesh->GetComponentLocation(), FRotator(0, 0, 0));
		g->path = r.PathData;
		g->targetLocation = end;
		g->targetSpace = space;
		g->grid = grid;
	}
	mesh->SetVisibility(false);
}
