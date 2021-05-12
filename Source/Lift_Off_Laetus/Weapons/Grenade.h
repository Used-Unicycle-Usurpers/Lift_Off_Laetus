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

	//The player who threw this grenade.
	class ACrewMember* owner;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//The sprint arm that holds the camera
	UPROPERTY(EditAnywhere)
		class USpringArmComponent* cameraArm;

	//The camera that follows this crew member
	UPROPERTY(EditAnywhere)
		class UCameraComponent* camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/**
	 * Explode this grenade, damaging all players in its range.
	 */
	void explode();

	/**
	 * Move the camera back the player, re-enable their input, and
	 * destroy this grenade.
	 */
	void destroySelf();

	//A reference to the game mode for quick access.
	class ALaetusGameMode* gameMode;

	//True if this grenade has already exploded, false otherwise.
	bool exploded;
};
