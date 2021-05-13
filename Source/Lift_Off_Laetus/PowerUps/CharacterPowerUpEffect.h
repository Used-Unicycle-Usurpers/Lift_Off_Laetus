
#pragma once

#include "CoreMinimal.h"
#include "BasePowerUpEffectComponent.h"
#include "Lift_Off_Laetus/Characters/CrewMember.h"
#include "CharacterPowerUpEffect.generated.h"

/**
 * Base class for effects applied to characters
 */
UCLASS(Abstract, BlueprintType)
class LIFT_OFF_LAETUS_API UCharacterPowerUpEffect : public UBasePowerUpEffectComponent
{
	GENERATED_BODY()
	
protected:
	ACrewMember* crewTarget;
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
};
