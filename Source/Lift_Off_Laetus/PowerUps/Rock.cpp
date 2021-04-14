// Fill out your copyright notice in the Description page of Project Settings.


#include "Rock.h"
#include "EnergizedOre.h"

ARock::ARock() {
	setHarvestSourceType(HarvestSourceType::Rock);
}

APowerUpActor* ARock::harvest() {
	return (APowerUpActor*) GetWorld()->SpawnActor<AEnergizedOre>(GetActorLocation(), GetActorRotation());
}

