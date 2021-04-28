// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUpActor.h"
#include "../GameManagement/GameEnums.h"
#include "HarvestSource.generated.h"

UCLASS()
class LIFT_OFF_LAETUS_API AHarvestSource : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHarvestSource();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Getter for the HarvestSourceType of this AHarvestSouce.
	HarvestSourceType getHarvestSourceType();

	/**
	 * TODO:
	 * Harvest the corresponding material from this space
	 */
	virtual APowerUpActor* harvest();

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Sets the HarvestSourceType of this AHarvestSource. Only child classes should
	//be able to set their type.
	void setHarvestSourceType(HarvestSourceType newType);

private:	
	//The type fo AHarvestSource this is (SlimeTree, Rock, or Shrub)
	HarvestSourceType type;

};
