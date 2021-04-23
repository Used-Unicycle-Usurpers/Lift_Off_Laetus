// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Grenade.generated.h"


UCLASS()
class LIFT_OFF_LAETUS_API AGrenade : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

	//Hitbox used to set the current occupant of this AGridSpace as players 
	//move around the board.
	UPROPERTY(EditAnywhere)
		class UCapsuleComponent* collision;

	//For visual debugging
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere)
		class UProjectileMovementComponent* movement;

	//The arc path to travel along
	TArray<FPredictProjectilePathPointData> path;
	
	//The current point this AGrenade is located at on the path
	int current;

	//The target map location and AGridSpace this grenade is travelling to.
	FVector targetLocation;
	class AGridSpace* targetSpace;

	//A reference to the map grid.
	class AGrid* grid;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
