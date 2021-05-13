// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePowerUpEffectComponent.h"
#include "../GameManagement/LaetusGameMode.h"

// Sets default values for this component's properties
UBasePowerUpEffectComponent::UBasePowerUpEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UBasePowerUpEffectComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBasePowerUpEffectComponent::DecrementLife(ALaetusGameMode* gameMode, int team, int playerID) {
	if (maxTurnLifetime != 0) {
		remainingTurns -= 1;

		if (remainingTurns <= 0) {
			gameMode->callHUDSetEffectOverlay(team, playerID, nullptr); // Incredibly cursed
			this->DestroyComponent();
		}
	}
}

void UBasePowerUpEffectComponent::ApplyEffect() { }

void UBasePowerUpEffectComponent::RemoveEffect() { }

