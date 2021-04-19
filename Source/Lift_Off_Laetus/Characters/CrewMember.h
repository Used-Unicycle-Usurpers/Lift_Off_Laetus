// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Weapons/Rifle.h"
#include "../Weapons/Launcher.h"
#include "../PowerUps/PowerUpEffect.h"
#include "CrewMember.generated.h"

UCLASS()
class LIFT_OFF_LAETUS_API ACrewMember : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACrewMember();

	//changed return from int to void for suggested functions
	void MoveTo(class AGridSpace * target);
	void Shoot(FVector direction);
	void Shove(); // what if there is more than one shove option
	void TakeDamage(int32 damage); //excluded cause parameter 

	class UStaticMeshComponent* Mesh;

	//For testing 
	//ACrewMember(int32 team);
	class UStaticMeshComponent* SphereMesh;
	float Speed;
	void SetTeam(int32 team);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Below is supposed to be the hitbox, needs testing
	//watch video to see what he says 

private:
	// Team character belongs too
	//ACrew* crew; 
	class ACrew* crew;

	// Character ID (index in crewMember array)
	int32 id;

	// Character properties 
	float health;
	int32 speed;
	int32 damage;

	// Character's connection to the world 
	class AGridSpace* gridSpace;

	//	Weapon info
	PowerUpEffect* gunEffect;
	//TODO: figure out how ot add weapons
	//ARifle * rifle;
	//ALauncher * launcher;

	//For testing 
	int32 team;
	
};
