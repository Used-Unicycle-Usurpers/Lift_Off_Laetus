// Fill out your copyright notice in the Description page of Project Settings.


#include "HarvestSource.h"
#include "PowerupEffectData.h"

// Sets default values
AHarvestSource::AHarvestSource() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HarvestSourceMesh"));
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(mesh);
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

/**
 * Returns the HarvestSourceType that this AHarvestSource is.
 * 
 * @return a HarvestSourceType enum representing the type of AHarvestSource 
 *     this is.
 */
HarvestSourceType AHarvestSource::getHarvestSourceType() {
	return type;
}

/**
 * Set the type of this AHarvestSource to the given HarvestSourceType.
 * 
 * @param newType a HarvestSourceType enum representing the type of AHarvestSource
 *     this is.
 */
void AHarvestSource::setHarvestSourceType(HarvestSourceType newType) {
	type = newType;
}

/**
 * TODO:
 * Harvest the corresponding material from this space
 * 
 * @return a pointer to an APowerUpActor representing the powerup that was
 *     just harvested by an ACrewMember.
 */
APowerUpActor* AHarvestSource::harvest() {
	return nullptr;
}

