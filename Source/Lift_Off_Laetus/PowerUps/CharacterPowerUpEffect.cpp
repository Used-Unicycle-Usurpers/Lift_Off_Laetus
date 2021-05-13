
#include "CharacterPowerUpEffect.h"

void UCharacterPowerUpEffect::OnRegister()
{
	Super::OnRegister();

	maxTurnLifetime = 3;
	remainingTurns = maxTurnLifetime;
	crewTarget = Cast<ACrewMember>(this->GetOwner());

	ApplyEffect();
}

void UCharacterPowerUpEffect::OnUnregister()
{
	Super::OnUnregister();

	RemoveEffect();
}