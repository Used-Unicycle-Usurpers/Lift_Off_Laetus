// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GridSpace.h"
#include "Lift_Off_LaetusGameMode.generated.h"

USTRUCT(BlueprintType)
struct FRow {
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
		TArray<int32> row;
};

USTRUCT(BlueprintType)
struct FGrid {
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
		TArray<FRow> rows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
		int NumRows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
		int NumColumns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
		FVector startingLocation;
};

UCLASS(minimalapi)
class ALift_Off_LaetusGameMode : public AGameModeBase {
	GENERATED_BODY()

private:
	struct FGrid grid;

	bool initializeGrid();

public:
	ALift_Off_LaetusGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};



