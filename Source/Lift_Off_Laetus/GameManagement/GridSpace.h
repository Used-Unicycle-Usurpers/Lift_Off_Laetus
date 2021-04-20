// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "../Characters/CrewMember.h"
#include "../PowerUps/HarvestSource.h"
#include "GridSpace.generated.h"

UCLASS()
class LIFT_OFF_LAETUS_API AGridSpace : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridSpace();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * Called when a new actor walks onto this AGridSpace. If it was a CrewMember, set it as
	 * the new occupant of this AGridSpace if is a valid actor (i.e. not pending kill and not
	 * NULL).
	 */
	UFUNCTION(BlueprintCallable)
		void OnEnterGridSpace(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**
	 * Called when the current occupant of this AGridSpace walks off. Occupant
	 * is set to nullptr.
	 */
	UFUNCTION(BlueprintCallable)
		void OnExitGridSpace(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	//Hitbox used to set the current occupant of this AGridSpace as players 
	//move around the board.
	UPROPERTY(EditAnywhere)
		class UBoxComponent* collision;

	//For visual debugging
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* mesh;

	//For visual debugging
	void SetToRed();
	void SetToBlue();
	void SetToGreen();

	//Get and set the occupant of this AGridSpace
	void setOccupant(AActor* newOccupant);
	AActor* getOccupant();

	//Return whether or not this AGridSpace is occupied
	bool isOccupied();

	//Getter and setter for the row and column of this AGridSpace
	void setGridLocation(int32 row, int32 column);
	FVector2D getGridLocation();

	//Getter and setter for AHarvestSource
	void setHarvestSource(AHarvestSource* newSource);
	AHarvestSource* getHarvestSource();

	//Apply/get/remove extra movement cost for this space
	void SetExtraMoveCost(int cost);
	int32 GetExtraMoveCost();
	void ResetExtraMoveCost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//The AActor (ACrewMember, ACoreFragment, etc.) currently occupting this AGridSpace
	AActor* occupant;

	//The row and column of this AGridSpace. The X component is 
	//the row and the Y component is the column.
	FVector2D gridLocation;

	//The AHarvestSource that occupants of this tile can harvest from
	//during their turn.
	AHarvestSource* harvestSource;

	//The additional cost of moving through this space
	int32 extraMoveCost = 0;
};
