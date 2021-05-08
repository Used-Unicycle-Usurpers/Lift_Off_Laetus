// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Launcher.generated.h"

/**
 * 
 */
UCLASS()
class LIFT_OFF_LAETUS_API ULauncher : public UWeapon {
	GENERATED_BODY()
	
public:
	ULauncher();

	/**
	 * Throws a grenade at the AGridSpace specified in direction.
	 */
	virtual int fire(FVector2D target) override;

	//The (row, column) coordinates of the target AGridSpace to throw the grenade to.
	FVector2D targetSpace;

	//The direction the ACrewMember will rotate to face before they throw the grenade.
	enum Direction directionToFaceEnum;

private:
	int range;

	/**
	 * Prepare to throw the grenade by starting the throw animation and setting a timer
	 * to later actually throw it.
	 */
	void readyLaunch();

	/**
	 * Caluclate a perfect arc from the player's location the target AGridSpace,
	 * and throw the grenade.
	 */
	void launch();

	/**
	 * Based on the coordinates of the given target AGridSpace, calculate which
	 * of the four caridinal directions to rotate to that most closely matches the
	 * direction you're throwing the grenade to. This is useful when throwing a
	 * grenade diagonally, so you can get which of the four directions most closely
	 * matches that direction to face.
	 */
	enum Direction getDirectionToThrow(FVector2D target);
};
