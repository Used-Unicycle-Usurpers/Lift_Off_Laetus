// Fill out your copyright notice in the Description page of Project Settings.


#include "HarvestSource.h"

// Sets default values
AHarvestSource::AHarvestSource() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HarvestSourceMesh"));
}

// Called when the game starts or when spawned
void AHarvestSource::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHarvestSource::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

HarvestSourceType AHarvestSource::getHarvestSourceType() {
	return type;
}

void AHarvestSource::setHarvestSourceType(HarvestSourceType newType) {
	type = newType;
}

APowerUpActor* AHarvestSource::harvest() {
	return nullptr;
}

