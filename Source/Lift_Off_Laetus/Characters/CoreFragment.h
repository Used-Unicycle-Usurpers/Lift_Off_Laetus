// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreFragment.generated.h"

/**
 * A core fragment teams need to push to their side of the map in 
 * order to get points.
 */
UCLASS()
class LIFT_OFF_LAETUS_API ACoreFragment : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoreFragment();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Static mesh of the core fragment.
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* mesh;

	/**
	 * Move this ACoreFragment to the provided AGridSpace. This movement
	 * is happening because this ACoreFragment is being pushed by the
	 * provided ACrewMember.
	 */
	void moveTo(class AGridSpace* target, class ACrewMember* pusher);

	/**
	 * Sets the AGridSpace this ACoreFragment is on to the provided
	 * AGridSpace.
	 */
	void setGridSpace(class AGridSpace* space);
	
	/**
	 * Returns the AGridSpace this ACoreFragment is currently standing on.
	 */
	class AGridSpace* getGridSpace();

	//The AGridSpace this core fragment is currently on.
	class AGridSpace* gridSpace;

private:
	/**
	 * Pushes this ACoreFragment from oldLoation to newLocation by
	 * incrementally pushing it in that direction until it reaches
	 * newLocation.
	 */
	void moveForward();

	/**
	 * Move this ACoreFragment in the direction specified in moveIncrement. This
	 * function is called by the looping time in moveForward to slowly move the
	 * ACoreFragment from oldLocation to newLocation.
	 */
	void incrementMoveForward();

	//Reference to the game's grid.
	class AGrid* grid;

	//Reference to the game mode.
	class ALaetusGameMode* gameMode;

	//The AGridSpace this ACoreFragment is being moved to.
	class AGridSpace* targetLocation;

	//The (row, column) this ACoreFragment is being moved to. 
	FVector newLocation;

	//The current (row, column) this ACoreFragment is located.
	FVector oldLocation;

	//The number of small increments to move this ACoreFragment by 
	//when moving from oldLocation to newLocation, so it looks like
	//it's being pushed.
	int numIncrements;

	//The number of increments left before this ACoreFragment has 
	//reached newLocation.
	int incrementsLeft;

	//Vector representing the amount this ACoreFragment moves during 
	//each increment, until it ultimately reaches newLocation.
	FVector moveIncrement;

	//FTimerHandle for the looping time that moves the ACoreFragment from
	//oldLocation to newLocation.
	FTimerHandle moveTimerHandle;

	class USoundCue* slidingSound;
};
