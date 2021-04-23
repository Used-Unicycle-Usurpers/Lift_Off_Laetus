// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterPowerUpEffect.h"
#include "CharacterSlipperyEffect.generated.h"

/**
 * Causes character to slip
 */
UCLASS()
class LIFT_OFF_LAETUS_API UCharacterSlipperyEffect : public UCharacterPowerUpEffect
{
	GENERATED_BODY()

protected:
	// Apply the effect
	virtual void ApplyEffect() override;

	// Remove the effect
	virtual void RemoveEffect() override;
};
