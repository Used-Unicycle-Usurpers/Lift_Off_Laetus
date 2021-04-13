// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GridSpace.h"
#include "Lift_Off_LaetusGameMode.generated.h"

#define VALID_IDX(idx, upperBound) idx > 0 && idx < upperBound

/**
 * Struct representing a single row of AGridSpaces in an FGrid.
 */
USTRUCT(BlueprintType)
struct FRow {
	GENERATED_BODY()
		
	//The value of each tile in this row of the map, as read in by the config file.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
		TArray<int32> rowNums;

	//The AGridSpace actors for this row of the map.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
		TArray<AGridSpace*> tiles;
};

/**
 * Struct representing the two-dimensional array of AGridSpaces matching
 * the game map.
 */
USTRUCT(BlueprintType)
struct FGrid {
	GENERATED_BODY()
	
	//The array of FRows, which each contains the AGridSpaces for their respective rows.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
		TArray<FRow> rows;

	//The number of rows in the grid i.e. length of rows.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
		int NumRows;

	//The number of columns in the grid i.e. the number of AGridSpaces in each FRow in rows.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
		int NumColumns;

	//The point in three-dimensional space to start placing AGridSpaces (i.e. top-left 
	//tile of the map grid). All AGridSpaces will be placed in relation to this point.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
		FVector startingLocation;
};

UCLASS(minimalapi)
class ALift_Off_LaetusGameMode : public AGameModeBase {
	GENERATED_BODY()

private:
	//The grid representing the tiles of the map.
	struct FGrid grid;

	/**
	 * Initialize the map tiles using the information in /Config/grid.txt. This loads
	 * the number for each tile into @bold grid struct, but will not spawn AGridSpace
	 * actors until BeginPlay.
	 */
	bool initializeGrid();

public:
	ALift_Off_LaetusGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Returns the AGridSpace that resides at the given coordinates, where the
	 * x value of the coordinates is the row index, the y value is the column
	 * index.
	 */
	AGridSpace* getTile(FVector2D coordinates);
};



