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

	//grid.rows.AddUninitialized(16);//Init(struct ALift_Off_LaetusGameMode::FRow, 16);
	for (int i = 0; i < 16; i++) {
		struct FRow row;
		for (int j = 0; j < 34; j++) {
			row.row.Add(j);
		}
		grid.rows.Add(row);
	}
}

void ALift_Off_LaetusGameMode::BeginPlay() {
	TArray<AActor*> results;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), results);
	ATargetPoint* startPoint = (ATargetPoint*)results[0];
	FVector start = startPoint->GetActorLocation();

	FCollisionQueryParams cqp;
	FHitResult hr;

	int tileWidth = 200;
	int numTilesWidth = 16;
	int tileLength = 200;
	int numTilesLength = 34;

	for (int i = 0; i < numTilesWidth; i++) {
		for (int j = 0; j < numTilesLength; j++) {
			FVector startHeight = FVector(start.X + j * tileLength, start.Y + i * tileWidth, 600);
			FVector endHeight = FVector(start.X + j * tileLength, start.Y + i * tileWidth, -600);
			FVector location = FVector(start.X + j * tileLength, start.Y + i * tileWidth, 600);
			FRotator rotation = FRotator(0, 0, 0);
			GetWorld()->LineTraceSingleByChannel(hr, startHeight, endHeight, ECC_Visibility, cqp);
			if (hr.bBlockingHit == true) {
				if (hr.GetActor() != this) {
					location.Z = hr.ImpactPoint.Z + 10;
					AGridSpace* tile = GetWorld()->SpawnActor<AGridSpace>(location, rotation);
				}
			}
		}
	}

	initializeGrid();
	
	for (int i = 0; i < grid.rows.Num(); i++) {
		struct FRow row = grid.rows[i];
		for (int j = 0; j < row.row.Num(); j++) {
			UE_LOG(LogTemp, Warning, TEXT("Entry: %d"), row.row[j]);
		}
	}

}

bool ALift_Off_LaetusGameMode::initializeGrid() {
	TArray<FString> lines;
	FString name = FPaths::Combine(FPaths::ProjectDir(), TEXT("/Config/grid.txt"));
	FString delimeter = ",";
	FFileHelper::LoadFileToStringArray(lines, *name);
	grid.NumRows = FCString::Atoi(*lines[0]);
	grid.NumColumns = FCString::Atoi(*lines[1]);
	
	TArray<FString> locationStr;
	lines[2].ParseIntoArray(locationStr, *delimeter, false);
	grid.startingLocation.X = FCString::Atoi(*locationStr[0]);
	grid.startingLocation.Y = FCString::Atoi(*locationStr[1]);
	grid.startingLocation.Z = FCString::Atoi(*locationStr[2]);

	for (int i = 3; i < grid.NumRows; i++) {
		FString nextLine = lines[i];
		TArray<FString> rowStr;
		nextLine.ParseIntoArray(rowStr, *delimeter, false);
		for (int j = 0; j < grid.NumColumns; j++) {
			grid.rows[i].row[j] = FCString::Atoi(*rowStr[j]);
		}
	}

	return true;
}