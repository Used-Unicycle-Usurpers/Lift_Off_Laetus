// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupEffectData.h"
#include "CharacterPowerUpEffect.h"
#include "TilePowerUpEffect.h"
#include "Templates/SubclassOf.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "../Controllers/CrewController.h"
#include "../GameManagement/LaetusGameMode.h"
#include "Engine/Texture2D.h" 

void UPowerUpEffectData::ApplyCharacterEffect(ACrewMember* targetCrewMember) {

	if (targetCrewMember)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rifle target was valid"));

		UActorComponent* newEffect = NewObject<UActorComponent>(targetCrewMember, characterEffect.Get());

		if (newEffect && targetCrewMember->GetComponentByClass(characterEffect.Get()) == nullptr)
		{
			targetCrewMember->AddOwnedComponent(newEffect);
			newEffect->RegisterComponent();
			UE_LOG(LogTemp, Warning, TEXT("Rifle effect registered to %s"), *targetCrewMember->GetFName().ToString());
			
			targetCrewMember->getGameMode()->callHUDSetEffectOverlay(targetCrewMember->getTeam(), targetCrewMember->getID(), portraitOverlay);
		}
	}
}

void UPowerUpEffectData::ApplyTileEffect(AGridSpace* targetGridSpace) {
	
	if (targetGridSpace)
	{
		UE_LOG(LogTemp, Warning, TEXT("Grenade target was valid"));

		UActorComponent* newEffect = NewObject<UActorComponent>(targetGridSpace, tileEffect.Get());

		if (newEffect)
		{
			targetGridSpace->AddOwnedComponent(newEffect);
			newEffect->RegisterComponent();
			UE_LOG(LogTemp, Warning, TEXT("Grenade effect registered to %s"), *targetGridSpace->GetFName().ToString());

			targetGridSpace->mesh->SetMaterial(0, tileMaterial);

			if (UGameplayStatics::SpawnEmitterAtLocation(targetGridSpace->GetWorld(), impactParticles, targetGridSpace->GetActorTransform(), false) != nullptr)
				UE_LOG(LogTemp, Warning, TEXT("Particles spawned at %s"), *targetGridSpace->GetFName().ToString());
		}
	}
}