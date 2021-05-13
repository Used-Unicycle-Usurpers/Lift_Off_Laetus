// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
#include "../PowerUps/SlimeTree.h"
#include "../PowerUps/Rock.h"
#include "../PowerUps/Shrub.h"
#include "../Characters/CoreFragment.h"
#include "CoreFragmentReceiver.h"

// Sets default values
AGrid::AGrid() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGrid::BeginPlay() {
	Super::BeginPlay();

	//Read in the config file and store the 2D array of integers that detail
	//what each tile contains.
	initializeGrid();

	//Now place a grid space over each map tile based on the info loaded in
	//from /Config/grid.txt during intializeGrid().
	placeGridSpaces();

	//Now place the harvest sources based the infomration in /Config/grid_env.txt.
	placeEnvironmentObjects();

	//Now place all the core fragments specified in /Config/grid_fragments.txt
	placeCoreFragments();

	//Now the map is ready, place the crew members at the GridSpaces specified in grid.txt
}

/**
 * Initialize the map tiles using the information in /Config/grid.txt. This loads
 * the number for each tile into @bold grid struct, but will not spawn AGridSpace
 * actors until BeginPlay.
 */
bool AGrid::initializeGrid() {
	TArray<FString> lines;
	FString name = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("ConfigFiles/grid.txt"));
	FString delimeter = ",";

	//Parse the lines of the file into an array of strings
	FFileHelper::LoadFileToStringArray(lines, *name);

	//The first two lines should be the number of rows and columns, respectively
	numRows = FCString::Atoi(*(lines[0].RightChop(8)));
	numColumns = FCString::Atoi(*(lines[1].RightChop(11)));
	tileHeight = FCString::Atoi(*(lines[2].RightChop(11)));
	numSteps = FCString::Atoi(*(lines[3].RightChop(9)));

	//Get the rows of the grid to start players out on
	TArray<FString> startingRowsStr;
	lines[4].RightChop(13).ParseIntoArray(startingRowsStr, *delimeter, false);
	startingRows.Add(FCString::Atoi(*startingRowsStr[0]));
	startingRows.Add(FCString::Atoi(*startingRowsStr[1]));
	startingRows.Add(FCString::Atoi(*startingRowsStr[2]));

	//Break the third line up into the three coordinates of the starting location
	//(i.e. the location to place the first, top left tile).
	TArray<FString> locationStr;
	lines[5].RightChop(17).ParseIntoArray(locationStr, *delimeter, false);
	startingLocation.X = FCString::Atoi(*locationStr[0]);
	startingLocation.Y = FCString::Atoi(*locationStr[1]);
	startingLocation.Z = FCString::Atoi(*locationStr[2]);

	//Remainder of the file is an 2D array of the different tiles of the map. The
	//numbers encode details about that space, add the number here and the 
	//corresponding tiles will be spawned at BeginPlay.
	for (int i = 0; i < numRows; i++) {
		FString nextLine = lines[i + 6];
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
		AGridSpace* tile = rows[coordinates.X].tiles[coordinates.Y];
		if (IsValid(tile)) {
			return tile;
		}
		else {
			return nullptr;
		}
	}
	else {
		return nullptr;
	}
};

/**
 * Given the information in the TArray rows (loaded with information from 
 * /Config/grid.txt during intializeGrid()), place a GridSpace actor on each 
 * tile of the map.
 */
void AGrid::placeGridSpaces() {
	FVector start = startingLocation;

	int tileWidth = 200;
	int numTilesWidth = numRows;
	int tileLength = 200;
	int numTilesLength = numColumns;

	FCollisionQueryParams cqp;
	FHitResult hr;

	// Find the midpoint of the board along y-axis
	int halfWidth = numTilesWidth / 2;

	//One by one, place grid space according to the information contained in the
	//config file.
	for (int i = 0; i < numTilesWidth; i++) {
		for (int j = 0; j < numTilesLength; j++) {
			int configInfo = rows[i].rowNums[j];

			if (configInfo != 0 && configInfo != 5) {
				//Do a line trace down to place the GridSpace just a few units above the groud.
				FVector location = FVector(start.X + j * tileLength, start.Y + i * tileWidth, tileHeight);
				FVector startHeight = FVector(start.X + j * tileLength, start.Y + i * tileWidth, 600);
				FVector endHeight = FVector(start.X + j * tileLength, start.Y + i * tileWidth, -600);
				GetWorld()->LineTraceSingleByChannel(hr, startHeight, endHeight, ECC_Visibility, cqp);
				if (hr.bBlockingHit == true && hr.GetActor() != this) {
						location.Z = hr.ImpactPoint.Z;
				}

				FRotator rotation = FRotator(0, 0, 0);

				AGridSpace* tile;

				if ( ((i == 0) || ((i + 1) % 4 == 0)) && (j == 0 || j == numTilesLength - 1)) {
					
					ACoreFragmentReceiver* coreRec = GetWorld()->SpawnActor<ACoreFragmentReceiver>(location, rotation);
					tile = coreRec;

					if (j == 0) {
						receiver0 = coreRec;
					} else {
						receiver1 = coreRec;
					}

				} else {
					tile = GetWorld()->SpawnActor<AGridSpace>(location, rotation);
				}

				tile->setGridLocation(i, j);

				//Finally, store the reference in the grid for easy access later
				rows[i].tiles.Add(tile);
			}
			else {
				//Use nullptr to make spots on the grid where there is no tile 
				//i.e. a hole in the map.
				rows[i].tiles.Add(nullptr);
			}
		}
	}
}

void AGrid::assignCoreFragmentReceivers(ACrew* crew0, ACrew* crew1) {
	receiver0->SetCrew(crew0);
	receiver1->SetCrew(crew1);
}

/**
 * Given the infomation in /Config/grid_env.txt, place each of the specified
 * HarvestSources in the map, passing a reference to that actor to all of the
 * neighboring tiles that will be able to harvest from it.
 */
void AGrid::placeEnvironmentObjects() {
	TArray<FString> lines;
	FString name = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("/ConfigFiles/grid_env.txt"));
	FString delimeter = "|";

	//Parse the lines of the file into an array of strings
	FFileHelper::LoadFileToStringArray(lines, *name);
	
	//Line by line, place each HarvestSource at the specified location.
	for (int i = 0; i < lines.Num(); i++) {
		FString nextLine = lines[i];
		TArray<FString> rowStr;
		nextLine.ParseIntoArray(rowStr, *delimeter, false);//Divide into tokens by "|"

		//The type to be substring after the "type=" in the config file.
		HarvestSourceType type = intToHarvestSourceType(FCString::Atoi(*rowStr[0].RightChop(5)));
		
		//Average the coordinates of the tiles to place between all of them.
		TArray<FVector2D> coordinates;
		TArray<FString> coordStrs;
		rowStr[1].RightChop(6).ParseIntoArray(coordStrs, TEXT(","), false);//Substring after "tiles="
		for (int j = 0; j < coordStrs.Num(); j+=2) {
			coordinates.Add(FVector2D(FCString::Atoi(*coordStrs[j]), FCString::Atoi(*coordStrs[j + 1])));
		}
		FVector2D gridLocation = averageCoordinates(coordinates);
		FVector mapLocation = FVector(gridLocation.X, gridLocation.Y, 0);

		//Run a line trace to place it just above the ground
		FCollisionQueryParams cqp;	
		FHitResult hr;
		FVector startHeight = FVector(gridLocation.X, gridLocation.Y, 600);
		FVector endHeight = FVector(gridLocation.X, gridLocation.Y, -600);
		GetWorld()->LineTraceSingleByChannel(hr, startHeight, endHeight, ECC_Visibility, cqp);
		if (hr.bBlockingHit == true && hr.GetActor() != this) {
			mapLocation.Z = hr.ImpactPoint.Z;
		}

		//Now spawn the HarvestSource, and keep a reference to pass to neigboring tiles
		AHarvestSource* source = nullptr;
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

		//Access neighoring tiles, and pass a reference to source so they can harvest
		//from this source during the game.
		TArray<FString> neighbrCoords;
		rowStr[2].RightChop(10).ParseIntoArray(neighbrCoords, TEXT(","), false);//substring after "neighbors="
		for (int j = 0; j < neighbrCoords.Num(); j += 2) {
			int row = FCString::Atoi(*neighbrCoords[j]);
			int column = FCString::Atoi(*neighbrCoords[j + 1]);

			AGridSpace* neighbor = getTile(FVector2D(row, column));
			if (IsValid(neighbor) && IsValid(source)) {
				neighbor->setHarvestSource(source);
			}else {
				UE_LOG(LogTemp, Warning, TEXT("Error: invalid tile coordinates: (%d,%d)"), row, column);
			}
		}
	}
}

void AGrid::placeCoreFragments() {
	TArray<FString> lines;
	FString name = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("ConfigFiles/grid_fragments.txt"));
	FString delimeter = ",";

	FCollisionQueryParams cqp;
	FHitResult hr;

	//Parse the lines of the file into an array of strings
	FFileHelper::LoadFileToStringArray(lines, *name);
	for (FString nextLine : lines) {
		TArray<FString> rowStr;
		nextLine.ParseIntoArray(rowStr, *delimeter, false);//Divide into tokens by "|"

		for (int i = 0; i < rowStr.Num(); i += 2) {
			int row = FCString::Atoi(*rowStr[i]);
			int column = FCString::Atoi(*rowStr[i+1]);
			AGridSpace* space = getTile(FVector2D(row, column));

			ACoreFragment* fragment = GetWorld()->SpawnActor<ACoreFragment>(space->GetActorLocation(), FRotator(0.f, 0.f, 0.f));

			fragment->setGridSpace(space);
		}
	}

}

/**
 * With the given array of (row, column) coordinates, average together all the 
 * row (x) components and all the column (y) components in world coordinates.
 * 
 * @param coordinates a TArray of FVecto2D coordinates, where the X component is 
 *     row index and the Y component is the column index.
 * @return a single FVector2D in which the X component is the average of the X
 *     components in coordinates, and the Y component is the average of the Y
 *     components in coordinates.
 */
FVector2D AGrid::averageCoordinates(TArray<FVector2D> coordinates) {
	int rowSum = 0;
	int columnSum = 0;
	for (int i = 0; i < coordinates.Num(); i++) {
		//Confusing, but for coordinates, X is the offset by row, which means we need
		//to move along the Y axis in world coordinates, and vice versa for column
		rowSum += (startingLocation.X + coordinates[i].Y * 200);
		columnSum += (startingLocation.Y + coordinates[i].X * 200);
	}
	return FVector2D(rowSum / coordinates.Num(), columnSum / coordinates.Num());
}

/**
 * Convert the given interger type to the corresponding
 * HarvestSourceType enum.
 *
 * @param type the integer representation of the type of harvest source
 * @return the HarvestSourceType enum that corresponds to type,
 *     Invalid if @bold type cannot be converted to a valid HarvestSourceType
 *     (outside range of enum)
 */
HarvestSourceType AGrid::intToHarvestSourceType(int type) {
	switch (type) {
	case SLIME_TREE:
		return SlimeTree;
	case ROCK:
		return Rock;
	case SHRUB:
		return Shrub;
	default:
		return Invalid;
	}
}

/**
 * Returns the array of rows to start the crew members on.
 * 
 * @return an array of 3 integers, representing the rows to place
 *     the three crew members at the beginning of the game.
 */
TArray<int32> AGrid::getStartingRows() {
	return startingRows;
}

/**
 * Returns the number of steps on either side of the map (should
 * be symetrical).
 * 
 * @return the number of steps on either side of the map.
 */
int AGrid::getNumSteps() {
	return numSteps;
}

/**
 * Returns the number of columns in the map grid.
 * 
 * @return an integer representing the number of columns in the map grid.
 */
int AGrid::getNumColumns() {
	return numColumns;
}

// Called every frame
void AGrid::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

FVector2D AGrid::getUnitDifference(AGridSpace* source, AGridSpace* dest) {
	FVector2D sourceGridLocation = source->getGridLocation();
	FVector2D destGridLocation = dest->getGridLocation();
	return destGridLocation - sourceGridLocation;
}

AGridSpace* AGrid::getValidRespawnSpace(ACrewMember* crewMember) {
	int column = 0;
	if (crewMember->getTeam() == 1) {
		column = numColumns - 1;
	}

	bool spaceFound = false;
	while (!spaceFound) {
		int randRow = FMath::RandRange(0, numRows - 1);
		AGridSpace* space = getTile(FVector2D(randRow, column));
		if (space && !space->isOccupied()) {
			spaceFound = true;
			return space;
		}
	}
	return nullptr;
}

void AGrid::colorGridInRange(FVector2D origin, int range) {
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numColumns; j++) {
			AGridSpace* tile = getTile(FVector2D(i, j));
			if (tile) {
				int dX = FMath::Abs(origin.X - i);
				int dY = FMath::Abs(origin.Y - j);
				if (dX <= range && dY <= range) {	
					tile->SetOverlayToBlue(false);
				}
			}
		}
	}
}

void AGrid::colorGridDirectionsInRange(const FVector2D origin, int range) {
	UE_LOG(LogTemp, Warning, TEXT("Origin is: (%f,%f)"), origin.X, origin.Y);
	for (int i = origin.X - range; i < origin.X + range+1; i++) {
		UE_LOG(LogTemp, Warning, TEXT("Getting tile at (%d,%f)"), i, origin.Y);
		AGridSpace* tile = getTile(FVector2D(i, origin.Y));
		if (tile) {
				tile->SetOverlayToBlue(false);
		}
	}

	for (int j = origin.Y - range; j < origin.Y + range+1; j++) {
		AGridSpace* tile = getTile(FVector2D(origin.X, j));
		if (tile) {
			tile->SetOverlayToBlue(false);
		}
	}
}

void AGrid::clearGridOverlay() {
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numColumns; j++) {
			AGridSpace* tile = getTile(FVector2D(i, j));
			if (tile) {
				tile->ClearOverlay();
			}
		}
	}
}

bool AGrid::areTilesWithinRange(FVector2D loc1, FVector2D loc2, int range) {
	int dX = FMath::Abs(loc1.X - loc2.X);
	int dY = FMath::Abs(loc1.Y - loc2.Y);
	return (dX <= range && dY <= range);
}

bool AGrid::canMove(AGridSpace* location, FVector2D direction) {
	FVector2D target = location->getGridLocation() + direction;
	AGridSpace* dest = getTile(target);
	if (dest) {
		if (dest->containsFragment()) {
			FVector2D fragTarget = target + direction;
			return !getTile(fragTarget)->isOccupied();
		}else {
			return !getTile(target)->isOccupied();
		}
	}else {
		return false;
	}
}