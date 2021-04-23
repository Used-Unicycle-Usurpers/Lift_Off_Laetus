// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TilePowerUpEffect.h"
#include "TileSlipperyEffect.generated.h"

/**
 * Makes a tile slippery
 */
UCLASS()
class LIFT_OFF_LAETUS_API UTileSlipperyEffect : public UTilePowerUpEffect
{
	GENERATED_BODY()
	
protected:
	// Apply the effect
	virtual void ApplyEffect() override;

	// Remove the effect
	virtual void RemoveEffect() override;
};
