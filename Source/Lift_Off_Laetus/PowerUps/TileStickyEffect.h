// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TilePowerUpEffect.h"
#include "TileStickyEffect.generated.h"

/**
 * Applies a sticky effect to ground tiles
 */
UCLASS()
class LIFT_OFF_LAETUS_API UTileStickyEffect : public UTilePowerUpEffect
{
	GENERATED_BODY()

public:
	// Apply the effect
	virtual void ApplyEffect() override;

	// Remove the effect
	virtual void RemoveEffect() override;

	// Apply and remove effects on characters walking over
	virtual void ApplyCharacterEffect(ACrewMember* crewMember) override;
	virtual void RemoveCharacterEffect(ACrewMember* crewMember) override;
};
