// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
#include "../PowerUps/SlimeTree.h"
#include "../PowerUps/Rock.h"
#include "../PowerUps/Shrub.h"

static TArray<UStaticMesh*> treeTrunks;
static TArray<FString> treeTrunkReferences = {
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__TreeBase.sampleMap_with_Environment__1__TreeBase'",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__TreeBase1.sampleMap_with_Environment__1__TreeBase1",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__TreeBase2.sampleMap_with_Environment__1__TreeBase2'",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__TreeBase3.sampleMap_with_Environment__1__TreeBase3'",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__TreeBase4.sampleMap_with_Environment__1__TreeBase4'",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__TreeBase5.sampleMap_with_Environment__1__TreeBase5'",
	"StaticMesh'/Game/Geometry/MapPieces/sampleMap_with_Environment__1__TreeBase6.sampleMap_with_Environment__1__TreeBase6'",
};

// Sets default values
AGrid::AGrid() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	for (int i = 0; i < treeTrunkReferences.Num(); i++) {
		static ConstructorHelpers::FObjectFinder<UStaticMesh>temp(*treeTrunkReferences[i]);
		treeTrunks.Add(temp.Object);
	}

}

// Called when the game starts or when spawned
void AGrid::BeginPlay() {
	//Read in the config file and store the 2D array of integers that detail
	//what each tile contains.
	initializeGrid();

	placeGridSpaces();

	placeEnvironmentObjects();
		
	//Now go through and spawn each harvest source, and provide a refernce to each
	//surrounding tile that will be able to harvest from it.
	/*
	for (int i = 0; i < numTilesWidth; i++) {
		for (int j = 0; j < numTilesLength; j++) {
			int configInfo = rows[i].rowNums[j];
			switch (configInfo) {
			case SLIME_TREE:
				placeSlimeTree(i, j);
				break;
			case ROCK:
				placeRock(i, j);
				break;	
			case SHRUB:
				placeShrub(i, j);
				break;
			}
		}
	}
	*/
}

void AGrid::placeGridSpaces() {
	FVector start = startingLocation;

	int tileWidth = 200;
	int numTilesWidth = numRows;
	int tileLength = 200;
	int numTilesLength = numColumns;

	//One by one, place grid space according to the information contained in the
	//config file.
	for (int i = 0; i < numTilesWidth; i++) {
		for (int j = 0; j < numTilesLength; j++) {
			int configInfo = rows[i].rowNums[j];

			if (configInfo != 0 && configInfo != 5) {
				//Calculate location of the next AGridSpace
				FVector location = FVector(start.X + j * tileLength, start.Y + i * tileWidth, tileHeight);
				FRotator rotation = FRotator(0, 0, 0);
				AGridSpace* tile = GetWorld()->SpawnActor<AGridSpace>(location, rotation);
				tile->setGridLocation(i, j);

				//For debug purposes, set color of the tiles
				switch (configInfo) {
				case 2:
					tile->SetToRed();
					break;
				case 3:
					tile->SetToBlue();
					break;
				case 4:
					tile->SetToGreen();
					break;
				}

				//Finally, store the reference in the grid for easy access later
				rows[i].tiles.Add(tile);
			}
		}
	}
}

void AGrid::placeEnvironmentObjects() {
	TArray<FString> lines;
	FString name = FPaths::Combine(FPaths::ProjectDir(), TEXT("/Config/grid_env.txt"));
	FString delimeter = ",";

	//Parse the lines of the file into an array of strings
	FFileHelper::LoadFileToStringArray(lines, *name);
	
	for (int i = 0; i < lines.Num(); i++) {
		FString nextLine = lines[i];
		TArray<FString> rowStr;
		nextLine.ParseIntoArray(rowStr, *delimeter, false);

		HarvestSourceType type = intToHarvestSourceType(FCString::Atoi(*rowStr[0]));
		int numTiles = FCString::Atoi(*rowStr[1]);
		
		//Average the coordinates of the tiles to place between all of them
		TArray<FVector2D> coordinates;
		for (int j = 2; j < (numTiles * 2)+1; j+=2) {
			coordinates.Add(FVector2D(FCString::Atoi(*rowStr[j]), FCString::Atoi(*rowStr[j + 1])));
		}
		FVector2D gridLocation = averageCoordinates(coordinates);
		FVector mapLocation = FVector(gridLocation.X, gridLocation.Y, 0);

		//Spawn and keep a reference
		AHarvestSource* source;
		switch (type) {
		case SlimeTree:
			source = GetWorld()->SpawnActor<ASlimeTree>(mapLocation, FRotator(0, 0, 0));
			break;
		case Rock:
			source = GetWorld()->SpawnActor<ARock>(mapLocation, FRotator(0, 0, 0));
			break;
		case Shrub:
			source = GetWorld()->SpawnActor<AShrub>(mapLocation, FRotator(0, 0, 0));
			break;
		}
		 
		//Pass reference to the surrounding tiles

	}
}

HarvestSourceType AGrid::intToHarvestSourceType(int type) {
	switch(type) {
		case SLIME_TREE:
			return SlimeTree;
		case ROCK:
			return Rock;
		case SHRUB:
			return Shrub;
		default:
			return Shrub;
	}
}

FVector2D AGrid::averageCoordinates(TArray<FVector2D> coordinates) {
	int rowSum = 0;
	int columnSum = 0;
	for (int i = 0; i < coordinates.Num(); i++) {
		//Confusing, but for coordinates, X is the offset by row, which means we need
		//to move along the Y axis in world coordaintes, and vice versa for column
		rowSum += (startingLocation.X + coordinates[i].Y * 200);
		columnSum += (startingLocation.Y + coordinates[i].X * 200);
	}
	return FVector2D(rowSum / coordinates.Num(), columnSum / coordinates.Num());
}

// Called every frame
void AGrid::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}


/**
 * Initialize the map tiles using the information in /Config/grid.txt. This loads
 * the number for each tile into @bold grid struct, but will not spawn AGridSpace
 * actors until BeginPlay.
 */
bool AGrid::initializeGrid() {
	TArray<FString> lines;
	FString name = FPaths::Combine(FPaths::ProjectDir(), TEXT("/Config/grid.txt"));
	FString delimeter = ",";

	//Parse the lines of the file into an array of strings
	FFileHelper::LoadFileToStringArray(lines, *name);

	//The first two lines should be the number of rows and columns, respectively
	numRows = FCString::Atoi(*lines[0]);
	numColumns = FCString::Atoi(*lines[1]);
	tileHeight = FCString::Atoi(*lines[2]);

	//Break the third line up into the three coordinates of the starting location
	//(i.e. the location to place the first, top left tile).
	TArray<FString> locationStr;
	lines[3].ParseIntoArray(locationStr, *delimeter, false);
	startingLocation.X = FCString::Atoi(*locationStr[0]);
	startingLocation.Y = FCString::Atoi(*locationStr[1]);
	startingLocation.Z = FCString::Atoi(*locationStr[2]);

	//Remainder of the file is an 2D array of the different tiles of the map. The
	//numbers encode details about that space, add the number here and the 
	//corresponding tiles will be spawned at BeginPlay.
	for (int i = 0; i < numRows; i++) {
		FString nextLine = lines[i + 4];
		TArray<FString> rowStr;
		nextLine.ParseIntoArray(rowStr, *delimeter, false);
		struct FRow newRow;
		for (int j = 0; j < numColumns; j++) {
			newRow.rowNums.Add(FCString::Atoi(*rowStr[j]));
		}
		rows.Add(newRow);
	}

	return true;
}

/**
 * Returns the AGridSpace that resides at the given coordinates, where the
 * x value of the coordinates is the row index, the y value is the column
 * index.
 *
 * @param coordinates the row and column number of the desired AGridSpace
 * @return the AGridSpace that resides at the given coordinates, nullptr if the
 * given row and column do not exist in the confines of the map, or if there is
 * no tile there (i.e. a hole in the map).
 */
AGridSpace* AGrid::getTile(FVector2D coordinates) {
	if (VALID_IDX(coordinates.X, numRows) && VALID_IDX(coordinates.Y, numColumns)) {
		return rows[coordinates.X].tiles[coordinates.Y];
	}else {
		return nullptr;
	}
};