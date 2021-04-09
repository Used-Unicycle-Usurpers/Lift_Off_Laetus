// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CrewMember.generated.h"

UCLASS()
class LIFT_OFF_LAETUS_API ACrewMember : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACrewMember();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
