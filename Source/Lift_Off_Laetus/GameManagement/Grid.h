// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridSpace.h"
#include "../PowerUps/HarvestSource.h"
#include "Grid.generated.h"

#define VALID_IDX(idx, upperBound) idx >= 0 && idx < upperBound
#define HARVEST_SLIME_TREE 2
#define HARVEST_ROCK 3
#define HARVEST_SHRUB 4
#define SLIME_TREE 5
#define ROCK 6
#define SHRUB 7

class ACrew;

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

UCLASS()
class LIFT_OFF_LAETUS_API AGrid : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * Initialize the map tiles using the information in /Config/grid.txt. This loads
	 * the number for each tile into @bold grid struct, but will not spawn AGridSpace
	 * actors until BeginPlay.
	 */
	bool initializeGrid();

	/**
	* Initialize the ACoreFragmentReceiver grid spaces with their respective crew identity.
	*/
	void assignCoreFragmentReceivers(ACrew* crew0, ACrew* crew1);

	/**
	 * Returns the AGridSpace that resides at the given coordinates, where the
	 * x value of the coordinates is the row index, the y value is the column
	 * index.
	 */
	AGridSpace* getTile(FVector2D coordinates);

	/**
	 * Returns the array of rows to start the crew members on.
	 */
	TArray<int32> getStartingRows();

	/**
	 * Returns the number of steps on either side of the map (should
	 * be symetrical).
	 */
	int getNumSteps();

	/**
	 * Returns the number of columns in the map grid.
	 */
	int getNumColumns();

	FVector2D getUnitDifference(AGridSpace* source, AGridSpace* dest);

	AGridSpace* getValidRespawnSpace(ACrewMember* crewMember);

	void colorGridInRange(FVector2D origin, int range);

	void colorGridDirectionsInRange(FVector2D origin, int range);

	bool areTilesWithinRange(FVector2D loc1, FVector2D loc2, int range);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//The array of FRows, which each contains the AGridSpaces for their respective rows.
	UPROPERTY(EditAnywhere, Category = MyCategory)
		TArray<FRow> rows;

	//The number of rows in the grid i.e. length of rows.
	UPROPERTY(EditAnywhere, Category = MyCategory)
		int numRows;

	//The number of columns in the grid i.e. the number of AGridSpaces in each FRow in rows.
	UPROPERTY(EditAnywhere, Category = MyCategory)
		int numColumns;

	//The number of columns in the grid i.e. the number of AGridSpaces in each FRow in rows.
	UPROPERTY(EditAnywhere, Category = MyCategory)
		float tileHeight;

	//The number of columns in the grid i.e. the number of AGridSpaces in each FRow in rows.
	UPROPERTY(EditAnywhere, Category = MyCategory)
		TArray<int32> startingRows;

	//The number of columns in the grid i.e. the number of AGridSpaces in each FRow in rows.
	UPROPERTY(EditAnywhere, Category = MyCategory)
		int numSteps;

	//The point in three-dimensional space to start placing AGridSpaces (i.e. top-left 
	//tile of the map grid). All AGridSpaces will be placed in relation to this point.
	UPROPERTY(EditAnywhere, Category = MyCategory)
		FVector startingLocation;

	/**
	 * Given the information in the TArray rows (loaded with information from
	 * /Config/grid.txt during intializeGrid()), place a GridSpace actor on each
	 * tile of the map.
	 */
	void placeGridSpaces();

	class ACoreFragmentReceiver* receiver0;
	class ACoreFragmentReceiver* receiver1;

	/**
	 * Given the infomation in /Config/grid_env.txt, place each of the specified
	 * HarvestSources in the map, passing a reference to that actor to all of the
	 * neighboring tiles that will be able to harvest from it.
	 */
	void placeEnvironmentObjects();

	void placeCoreFragments();

	/**
	 * With the given array of (row, column) coordinates, average together all the
	 * row (x) components and all the column (y) components in world coordinates.
	 */
	FVector2D averageCoordinates(TArray<FVector2D> coordinates);

	/**
	 * Convert the given interger @bold type to the corresponding
	 * HarvestSourceType enum.
	 */
	HarvestSourceType intToHarvestSourceType(int type);
};
