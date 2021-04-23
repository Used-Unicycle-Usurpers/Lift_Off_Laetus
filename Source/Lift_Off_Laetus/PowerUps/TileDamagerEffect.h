// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TilePowerUpEffect.h"
#include "TileDamagerEffect.generated.h"

/**
 * Damages characters every time they step on the tile
 */
UCLASS()
class LIFT_OFF_LAETUS_API UTileDamagerEffect : public UTilePowerUpEffect
{
	GENERATED_BODY()

protected:
	// Apply the effect
	virtual void ApplyEffect() override;

	// Remove the effect
	virtual void RemoveEffect() override;
};
