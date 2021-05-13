// Fill out your copyright notice in the Description page of Project Settings.


#include "Shrub.h"
#include "ShrubFruit.h"
#include "EngineUtils.h"

static TArray<FString> shrubMeshReferences = {
	"StaticMesh'/Game/Geometry/Map/HarvestSourceMeshes/ENV_Shrub.ENV_Shrub'",
};

AShrub::AShrub() {
	setHarvestSourceType(HarvestSourceType::Shrub);

	TArray<UObject*> effects;
	EngineUtils::FindOrLoadAssetsByPath(TEXT("/Game/Effects"), effects, EngineUtils::ATL_Regular);

	for (auto asset : effects)
	{
		UPowerUpEffectData* effectType = Cast<UPowerUpEffectData>(asset);
		if (effectType != nullptr && effectType->effectName.IsEqual(TEXT("Slippery"))) {
			effectToGive = effectType;
			break;
		}
	}


	//Pick a random mesh
	int randomIdx = FMath::RandRange(0, shrubMeshReferences.Num() - 1);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>randMesh1(*shrubMeshReferences[0]);
	
	//Using a switch here so we can easily add more shrub meshes later.
	switch (randomIdx) {
	case 0:
	default:
		mesh->SetStaticMesh(randMesh1.Object);
	}

	//mesh->AttachToComponent(mesh, FAttachmentTransformRules::KeepRelativeTransform);
	mesh->SetWorldLocation(GetActorLocation());
	SetRootComponent(mesh);
}

APowerUpActor* AShrub::harvest() {
	return (APowerUpActor*) GetWorld()->SpawnActor<AShrubFruit>(GetActorLocation(), GetActorRotation());
}