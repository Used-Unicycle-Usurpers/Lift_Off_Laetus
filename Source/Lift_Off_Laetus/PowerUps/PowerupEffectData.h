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

	UPROPERTY(EditAnywhere, Category="General")
		FName effectName;

	UPROPERTY(EditAnywhere, Category = "General")
		FColor mainColor;

	UPROPERTY(EditAnywhere, Category="Character Effect")
		TSubclassOf<class UCharacterPowerUpEffect> characterEffect;

	UPROPERTY(EditAnywhere, Category = "Character Effect")
		UTexture2D* portraitOverlay;

	UPROPERTY(EditAnywhere, Category = "Tile Effect")
		TSubclassOf<class UTilePowerUpEffect> tileEffect;

	UPROPERTY(EditAnywhere, Category = "Tile Effect")
		UMaterial* tileMaterial;

	UPROPERTY(EditAnywhere, Category = "Tile Effect")
		UParticleSystem* impactParticles;


public:

	void ApplyCharacterEffect(class ACrewMember* targetCrewMember);
	void ApplyTileEffect(class AGridSpace* targetGridSpace);
};
