// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterPowerUpEffect.h"
#include "CharacterStickyEffect.generated.h"

/**
 * Applies a sticky effect to player characters
 */
UCLASS()
class LIFT_OFF_LAETUS_API UCharacterStickyEffect : public UCharacterPowerUpEffect
{
	GENERATED_BODY()
	
protected:
	// Apply the effect
	virtual void ApplyEffect() override;

	// Remove the effect
	virtual void RemoveEffect() override;
};
