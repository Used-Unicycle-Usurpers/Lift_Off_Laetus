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

	TArray<FPredictProjectilePathPointData> path;
	
	int current;
	FVector targetLocation;
	class AGridSpace* targetSpace;
	class AGrid* grid;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
