// Fill out your copyright notice in the Description page of Project Settings.


#include "TileStickyEffect.h"

void UTileStickyEffect::ApplyEffect()
{
	tileTarget->SetExtraMoveCost(1);
}

void UTileStickyEffect::RemoveEffect()
{
	tileTarget->SetExtraMoveCost(0);
}

