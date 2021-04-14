// Fill out your copyright notice in the Description page of Project Settings.


#include "SlimeTree.h"
#include "SlimeOrb.h"

ASlimeTree::ASlimeTree() {
	setHarvestSourceType(HarvestSourceType::SlimeTree);
}

APowerUpActor* ASlimeTree::harvest() {
	return (APowerUpActor*) GetWorld()->SpawnActor<ASlimeOrb>(GetActorLocation(), GetActorRotation());
}