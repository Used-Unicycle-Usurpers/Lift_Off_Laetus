// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class LIFT_OFF_LAETUS_API URifle : public UWeapon {
	GENERATED_BODY()
	
public:
	URifle();

	virtual int fire(FVector2D direction) override;
};
