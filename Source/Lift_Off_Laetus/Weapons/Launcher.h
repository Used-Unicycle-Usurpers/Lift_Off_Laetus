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
	virtual int fire(FVector2D direction) override;

	void readyLaunch();
	void launch();

	FVector2D targetDirection;

	FVector2D directionToThow;

	enum Direction directionToFaceEnum;

private:
	int range;
};
