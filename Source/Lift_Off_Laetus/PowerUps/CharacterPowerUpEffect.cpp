
#include "CharacterPowerUpEffect.h"

void UCharacterPowerUpEffect::OnRegister()
{
	crewTarget = Cast<ACrewMember>(this->GetOwner());

	ApplyEffect();
}

void UCharacterPowerUpEffect::OnUnregister()
{
	RemoveEffect();
}