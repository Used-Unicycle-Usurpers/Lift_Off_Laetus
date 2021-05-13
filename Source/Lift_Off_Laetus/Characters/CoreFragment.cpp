// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreFragment.h"
#include "../GameManagement/Grid.h"
#include "../GameManagement/GridSpace.h"
#include "../GameManagement/LaetusGameMode.h"
#include "../Characters/CrewMember.h"
#include "Kismet/GameplayStatics.h"
#include "../GameManagement/CoreFragmentReceiver.h"
#include "Sound/SoundCue.h"

// Sets default values
ACoreFragment::ACoreFragment() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);//Weird things happen with collision on, plus we don't need it anyways

	static ConstructorHelpers::FObjectFinder<UStaticMesh>CubeMeshAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/TEAM_CoreFragment__2_.TEAM_CoreFragment__2_'"));
	mesh->SetStaticMesh(CubeMeshAsset.Object);

	static ConstructorHelpers::FObjectFinder<USoundCue>sound(TEXT("SoundCue'/Game/Audio/AUD_core_slide01_Cue.AUD_core_slide01_Cue'"));
	slidingSound = sound.Object;
}

// Called when the game starts or when spawned
void ACoreFragment::BeginPlay() {
	Super::BeginPlay();

	gameMode = Cast<ALaetusGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode) {
		grid = gameMode->getGameGrid();
	}
}

// Called every frame
void ACoreFragment::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

/**
 * Move this ACoreFragment to the provided AGridSpace. This movement
 * is happening because this ACoreFragment is being pushed by the
 * provided ACrewMember.
 * 
 * @param target the AGridSpace to move this ACoreFragment to.
 * @param pusher the ACrewMember that is pushing this ACoreFragment.
 */
void ACoreFragment::moveTo(AGridSpace* target, ACrewMember* pusher) {
	if (target == nullptr || target->isOccupied()) {
		return;
	}
	
	targetLocation = target;	
	newLocation = targetLocation->GetActorLocation() + FVector(0, 0, 20);
	oldLocation = gridSpace->GetActorLocation() + FVector(0, 0, 20);

	//If the pusher needs to rotate before the begin pushing, we need to wait
	//on moving until they are ready to push.
	FVector2D unitDirection = grid->getUnitDifference(gridSpace, target);
	bool needsRotate = pusher->needToRotate(unitDirection);

	// Reset pointers/references. Do this now so the crew member pushing this
	// doesn't interpret this space as being occupied.
	setGridSpace(target);

	//Wait to move until the player has finished rotating (if they need to rotate)
	if (needsRotate) {
		FTimerHandle timerParams;
		GetWorld()->GetTimerManager().SetTimer(timerParams, this, &ACoreFragment::moveForward, 0.7f, false);
	}else {
		moveForward();
	}
}

/**
 * Pushes this ACoreFragment from oldLoation to newLocation by 
 * incrementally pushing it in that direction until it reaches 
 * newLocation.
 */
void ACoreFragment::moveForward() {
	//Calculate how much to increment movement by in each iteration of the timer.
	
	incrementsLeft = numIncrements = 60;
	moveIncrement = (newLocation - oldLocation) / numIncrements;

	UGameplayStatics::PlaySound2D(GetWorld(), slidingSound);

	//Start the timer to increment the position up until we reach the destination
	GetWorld()->GetTimerManager().SetTimer(moveTimerHandle, this, &ACoreFragment::incrementMoveForward, 0.01, true);
}

/**
 * Move this ACoreFragment in the direction specified in moveIncrement. This
 * function is called by the looping time in moveForward to slowly move the
 * ACoreFragment from oldLocation to newLocation.
 */
void ACoreFragment::incrementMoveForward() {
	FVector currentLocation = GetActorLocation();
	float distance = FVector::Dist(currentLocation, newLocation);

	//If in a certain distance tolerance of the actual location, consider 
	//the movement completed. This handles cases where moveIncrement does 
	//not add up to exactly the destination location.
	incrementsLeft--;
	if (incrementsLeft > 0) {
		//Destination has not been reached, increment position
		SetActorLocation(currentLocation + moveIncrement);
	}
	else {
		//Destination has been reached! Stop timer.
		SetActorLocation(newLocation);//Snap to the exact location
		GetWorld()->GetTimerManager().ClearTimer(moveTimerHandle);

		// Check if the core fragment has been pushed into a receiver
		if (targetLocation->getGridLocation().X == 0) {
		}

	}
}

/**
 * Sets the AGridSpace this ACoreFragment is on to the provided
 * AGridSpace.
 * 
 * @param space the AGridSpace this ACoreFragment is now standing on.
 */
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

/**
 * Returns the AGridSpace this ACoreFragment is currently standing on.
 */
AGridSpace* ACoreFragment::getGridSpace() {
	return gridSpace;
}