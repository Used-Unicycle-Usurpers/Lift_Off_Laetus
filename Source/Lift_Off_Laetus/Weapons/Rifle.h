// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class LIFT_OFF_LAETUS_API URifle : public UWeapon {
	GENERATED_BODY()
	
public:
	URifle();

	/**
	 * Fires a bullet (i.e. line trace) in the cardinal direction specified in
	 * target and damages the first player it finds (if any).
	 */
	virtual int fire(FVector2D target) override;

private:
	//The amount of damage a shot typically deals.
	float damage;

	//The range i.e. number of tiles forward the rifle can shoot.
	int range;

	//Reference to the game mode
	class ALaetusGameMode* gameMode;

	FVector2D directionToShoot;
	enum Direction directionToShootEnum;
	
	/**
	 * Fire the rifle in the current target direction up to range number of
	 * tiles. Stop either at max range or when the first ACrewMember gets hit.
	 */
	void shootRifle();
};
