// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePowerUpEffectComponent.h"
#include "Lift_Off_Laetus/GameManagement/GridSpace.h"
#include "TilePowerUpEffect.generated.h"

/**
 * Base class for effects applied to ground tiles
 */
UCLASS(Abstract, BlueprintType)
class LIFT_OFF_LAETUS_API UTilePowerUpEffect : public UBasePowerUpEffectComponent
{
	GENERATED_BODY()

public:
	virtual void ApplyCharacterEffect(ACrewMember* crewMember);
	virtual void RemoveCharacterEffect(ACrewMember* crewMember);
protected:
	AGridSpace* tileTarget;
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
};
