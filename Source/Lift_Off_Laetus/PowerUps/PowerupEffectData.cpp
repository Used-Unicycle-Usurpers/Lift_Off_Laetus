// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupEffectData.h"
#include "CharacterPowerUpEffect.h"
#include "TilePowerUpEffect.h"
#include "Templates/SubclassOf.h"
#include "Engine/EngineTypes.h"

void UPowerUpEffectData::ApplyCharacterEffect(ACrewMember* targetCrewMember) {

	if (targetCrewMember)
	{
		UActorComponent* newEffect = targetCrewMember->CreateComponentFromTemplate(Cast<UCharacterPowerUpEffect>(characterEffect.Get()));

		UE_LOG(LogTemp, Warning, TEXT("Target was valid"));

		if (newEffect)
		{
			UE_LOG(LogTemp, Warning, TEXT("Rifle effect was valid"));

			newEffect->RegisterComponent();
			targetCrewMember->AttachToActor(targetCrewMember, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void UPowerUpEffectData::ApplyTileEffect(AGridSpace* targetGridSpace) {
	
	if (targetGridSpace)
	{
		UActorComponent* newEffect = targetGridSpace->CreateComponentFromTemplate(Cast<UTilePowerUpEffect>(tileEffect.Get()));

		if (newEffect)
		{
			newEffect->RegisterComponent();
			targetGridSpace->AttachToActor(targetGridSpace, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}