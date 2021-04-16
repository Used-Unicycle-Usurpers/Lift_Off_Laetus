// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarvestSource.h"
#include "SlimeTree.generated.h"

/**
 * 
 */
UCLASS()
class LIFT_OFF_LAETUS_API ASlimeTree : public AHarvestSource {
	GENERATED_BODY()
	
public:
	ASlimeTree();

	virtual APowerUpActor* harvest() override;

	/*
	//For visual debugging
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* trunkMesh;

	//For visual debugging
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* bushMesh;
	*/
};
