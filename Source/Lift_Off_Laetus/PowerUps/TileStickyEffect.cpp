// Fill out your copyright notice in the Description page of Project Settings.


#include "TileStickyEffect.h"

void UTileStickyEffect::ApplyEffect()
{
	this->tileTarget->SetExtraMoveCost(1);
}

void UTileStickyEffect::RemoveEffect()
{
	this->tileTarget->SetExtraMoveCost(0);
}

