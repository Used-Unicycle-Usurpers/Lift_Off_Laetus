// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PowerupEffectData.generated.h"

/**
 * Used to create powerup descriptions in content browser
 */
UCLASS()
class LIFT_OFF_LAETUS_API UPowerUpEffectData : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FName effectName;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCharacterPowerUpEffect> characterEffect;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UTilePowerUpEffect> tileEffect;

	UPROPERTY(EditAnywhere)
		UTexture* portraitOverlay;

public:

	void ApplyCharacterEffect(class ACrewMember* targetCrewMember);
	void ApplyTileEffect(class AGridSpace* targetGridSpace);
};
