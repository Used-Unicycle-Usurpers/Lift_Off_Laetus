// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameManagement/Grid.h"
#include "Lift_Off_LaetusGameMode.generated.h"

UCLASS(minimalapi)
class ALift_Off_LaetusGameMode : public AGameModeBase {
	GENERATED_BODY()

private:
	//The grid representing the tiles of the map.
	//struct FGrid grid;
	UPROPERTY(EditAnywhere, Category = MyCategory)
		AGrid* grid;

public:
	ALift_Off_LaetusGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};



