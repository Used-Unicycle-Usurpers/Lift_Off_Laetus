// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUpEffect.h"
#include "PowerUpActor.generated.h"

UCLASS()
class LIFT_OFF_LAETUS_API APowerUpActor : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUpActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	PowerUpEffect* effect;

};
