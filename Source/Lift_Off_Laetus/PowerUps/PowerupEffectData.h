// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterPowerUpEffect.h"
#include "TilePowerUpEffect.h"
#include "PowerupEffectData.generated.h"

/**
 * Used to create powerup descriptions in content browser
 */
UCLASS()
class LIFT_OFF_LAETUS_API UPowerupEffectData : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UCharacterPowerUpEffect* characterEffect;

	UPROPERTY(EditAnywhere)
	UTilePowerUpEffect* tileEffect;
};
