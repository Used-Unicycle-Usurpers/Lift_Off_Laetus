// Fill out your copyright notice in the Description page of Project Settings.


#include "TileStickyEffect.h"

void UTileStickyEffect::ApplyEffect()
{
	UE_LOG(LogTemp, Warning, TEXT("Tile is sticky"));
	tileTarget->SetExtraMoveCost(1);
}

void UTileStickyEffect::RemoveEffect()
{
	UE_LOG(LogTemp, Warning, TEXT("Tile is not sticky"));
	tileTarget->SetExtraMoveCost(0);
}

void UTileStickyEffect::ApplyCharacterEffect(ACrewMember* crewMember)
{
	UE_LOG(LogTemp, Warning, TEXT("Character entered sticky tile"));

}

void UTileStickyEffect::RemoveCharacterEffect(ACrewMember* crewMember)
{
	UE_LOG(LogTemp, Warning, TEXT("Character left sticky tile"));

}

