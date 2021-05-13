// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStickyEffect.h"

void UCharacterStickyEffect::ApplyEffect()
{
	UE_LOG(LogTemp, Warning, TEXT("Character is sticky"));
	//this->crewTarget->SetExtraMoveCost(1);
}

void UCharacterStickyEffect::RemoveEffect()
{
	UE_LOG(LogTemp, Warning, TEXT("Character is not sticky"));
	//this->crewTarget->SetExtraMoveCost(0);
}