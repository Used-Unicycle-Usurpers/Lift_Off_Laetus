
#include "CharacterPowerUpEffect.h"

void UCharacterPowerUpEffect::OnRegister()
{
	Super::OnRegister();

	crewTarget = Cast<ACrewMember>(this->GetOwner());

	ApplyEffect();
}

void UCharacterPowerUpEffect::OnUnregister()
{
	Super::OnUnregister();

	RemoveEffect();
}