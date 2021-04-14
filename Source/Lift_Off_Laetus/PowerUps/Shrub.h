// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarvestSource.h"
#include "Shrub.generated.h"

/**
 * 
 */
UCLASS()
class LIFT_OFF_LAETUS_API AShrub : public AHarvestSource
{
	GENERATED_BODY()
	
public:
	AShrub();

	virtual APowerUpActor* harvest() override;

};
