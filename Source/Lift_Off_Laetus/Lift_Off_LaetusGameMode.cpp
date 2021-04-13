// Copyright Epic Games, Inc. All Rights Reserved.

#include "Lift_Off_LaetusGameMode.h"
#include "Lift_Off_LaetusPlayerController.h"
#include "Lift_Off_LaetusCharacter.h"
#include "Controllers/CrewController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "Misc/FileHelper.h"
#include "UObject/ConstructorHelpers.h"

ALift_Off_LaetusGameMode::ALift_Off_LaetusGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ACrewController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

/**
 * Called at the very beginning of the game. Sets up the map of AGridSpaces with
 * all the information contained in /Config/grid.txt.
 */
void ALift_Off_LaetusGameMode::BeginPlay() {
	//Read in the config file and store the 2D array of integers that detail
	//what each tile contains.
	initializeGrid();

	FVector start = grid.startingLocation;

	int tileWidth = 200;
	int numTilesWidth = grid.NumRows;
	int tileLength = 200;
	int numTilesLength = grid.NumColumns;

	//One by one, place grid space according to the information contained in the
	//config file.
	for (int i = 0; i < numTilesWidth; i++) {
		for (int j = 0; j < numTilesLength; j++) {
			int configInfo = grid.rows[i].rowNums[j];
			FCollisionQueryParams cqp;
			FHitResult hr;

			if (configInfo != 0 && configInfo != 5) {
				//Calculate location of the next AGridSpace
				FVector location = FVector(start.X + j * tileLength, start.Y + i * tileWidth, 11);
				FRotator rotation = FRotator(0, 0, 0);

				//Run a trace to place the AGridSpace just a few units above the ground
				FVector startHeight = FVector(start.X + j * tileLength, start.Y + i * tileWidth, 600);
				FVector endHeight = FVector(start.X + j * tileLength, start.Y + i * tileWidth, -600);
				GetWorld()->LineTraceSingleByChannel(hr, startHeight, endHeight, ECC_Visibility, cqp);
				if (hr.bBlockingHit == true) {
					if (hr.GetActor() != this) {
						//Hit a valid point of the map, spasn the AGridSpace
						location.Z = grid.tileHeight;
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
						grid.rows[i].tiles.Add(tile);
					}
				}
			}
		}
	}
}

/**
 * Initialize the map tiles using the information in /Config/grid.txt. This loads
 * the number for each tile into @bold grid struct, but will not spawn AGridSpace
 * actors until BeginPlay.
 */
bool ALift_Off_LaetusGameMode::initializeGrid() {
	TArray<FString> lines;
	FString name = FPaths::Combine(FPaths::ProjectDir(), TEXT("/Config/grid.txt"));
	FString delimeter = ",";
	
	//Parse the lines of the file into an array of strings
	FFileHelper::LoadFileToStringArray(lines, *name);

	//The first two lines should be the number of rows and columns, respectively
	grid.NumRows = FCString::Atoi(*lines[0]);
	grid.NumColumns = FCString::Atoi(*lines[1]);
	grid.tileHeight = FCString::Atoi(*lines[2]);
	
	//Break the third line up into the three coordinates of the starting location
	//(i.e. the location to place the first, top left tile).
	TArray<FString> locationStr;
	lines[3].ParseIntoArray(locationStr, *delimeter, false);
	grid.startingLocation.X = FCString::Atoi(*locationStr[0]);
	grid.startingLocation.Y = FCString::Atoi(*locationStr[1]);
	grid.startingLocation.Z = FCString::Atoi(*locationStr[2]);

	//Remainder of the file is an 2D array of the different tiles of the map. The
	//numbers encode details about that space, add the number here and the 
	//corresponding tiles will be spawned at BeginPlay.
	for (int i = 0; i < grid.NumRows; i++) {
		FString nextLine = lines[i+4];
		TArray<FString> rowStr;
		nextLine.ParseIntoArray(rowStr, *delimeter, false);
		struct FRow newRow;
		for (int j = 0; j < grid.NumColumns; j++) {
			newRow.rowNums.Add(FCString::Atoi(*rowStr[j]));
		}
		grid.rows.Add(newRow);
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
AGridSpace* ALift_Off_LaetusGameMode::getTile(FVector2D coordinates) {
	if (VALID_IDX(coordinates.X, grid.NumRows) && VALID_IDX(coordinates.Y, grid.NumColumns)) {
		return grid.rows[coordinates.X].tiles[coordinates.Y];
	}else {
		return nullptr;
	}
};
