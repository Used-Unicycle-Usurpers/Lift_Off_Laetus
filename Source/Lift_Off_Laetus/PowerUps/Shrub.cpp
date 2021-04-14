// Fill out your copyright notice in the Description page of Project Settings.


#include "Shrub.h"
#include "ShrubFruit.h"

AShrub::AShrub() {
	setHarvestSourceType(HarvestSourceType::Shrub);
}

APowerUpActor* AShrub::harvest() {
	return (APowerUpActor*) GetWorld()->SpawnActor<AShrubFruit>(GetActorLocation(), GetActorRotation());
}