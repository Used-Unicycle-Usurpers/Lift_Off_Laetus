// Fill out your copyright notice in the Description page of Project Settings.


#include "TilePowerUpEffect.h"

void UTilePowerUpEffect::OnRegister()
{
	Super::OnRegister();

	tileTarget = Cast<AGridSpace>(this->GetOwner());

	ApplyEffect();
}

void UTilePowerUpEffect::OnUnregister()
{
	Super::OnUnregister();

	RemoveEffect();
}

