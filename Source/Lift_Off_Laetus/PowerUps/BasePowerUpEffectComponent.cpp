// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePowerUpEffectComponent.h"

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

void UBasePowerUpEffectComponent::DecrementLife() {
	if (maxTurnLifetime != 0) {
		remainingTurns -= 1;

		if (remainingTurns <= 0) {
			this->DestroyComponent();
		}
	}
}

void UBasePowerUpEffectComponent::ApplyEffect() { }

void UBasePowerUpEffectComponent::RemoveEffect() { }

