// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUpActor.h"
#include "HarvestSource.generated.h"

UENUM()
enum HarvestSourceType {
	Rock,
	SlimeTree,
	Shrub
};

UCLASS()
class LIFT_OFF_LAETUS_API AHarvestSource : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHarvestSource();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	HarvestSourceType getHarvestSourceType();

	virtual APowerUpActor* harvest();

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void setHarvestSourceType(HarvestSourceType newType);

private:	
	HarvestSourceType type;

};
