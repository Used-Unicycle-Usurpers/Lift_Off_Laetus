// Fill out your copyright notice in the Description page of Project Settings.


#include "TilePowerUpEffect.h"

void UTilePowerUpEffect::OnRegister()
{
	tileTarget = Cast<AGridSpace>(this->GetOwner());

	ApplyEffect();
}

void UTilePowerUpEffect::OnUnregister()
{
	RemoveEffect();
}

