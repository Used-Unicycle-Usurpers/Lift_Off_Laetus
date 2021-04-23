// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Weapons/Rifle.h"
#include "../Weapons/Launcher.h"
#include "../PowerUps/PowerUpEffect.h"
#include "CrewMember.generated.h"

UCLASS()
class LIFT_OFF_LAETUS_API ACrewMember : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACrewMember();

	/**
	 * Move this ACrewMember to the given AGridSpace
	 */
	void MoveTo(class AGridSpace * target);
	
	/**
	 * Shoot one of this ACrewMember's weapons in the given direction
	 */
	void Shoot(FVector2D direction, bool useRifle);
	
	/**
	 * Shove the object that was in the AGridSpace this ACrewMember just moved
	 * into.
	 */
	void Shove(); // what if there is more than one shove option
	
	/**
	 * Reduce this ACrewMember's health by the given damage.
	 */
	void takeDamage(int32 damage); //excluded cause parameter 
	
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Mesh;
	
	//Default team color is red team's, so we need to save the 
	//other team's color in case they are assigned blue team
	UPROPERTY(EditAnywhere)
		class UMaterial* BlueTeamColor; //de
	
	class UStaticMeshComponent* SphereMesh;

	/**
	 * Set this ACrewMember's team to the given team
	 */
	void SetTeam(int32 team);

	/**
	 * Set the AGridSpace this ACrewMember is currently standing on to the given
	 * AGridSpace.
	 */
	void setGridSpace(class AGridSpace* space);

	/**
	 * Get the AGridSpace this ACrewMember is currently standing on.
	 */
	class AGridSpace* getGridSpace();

	/**
	 * Returns the ACrew (team) this ACrewMember is a part of.
	 */
	class ACrew* getCrew();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Below is supposed to be the hitbox, needs testing
	//watch video to see what he says 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// The Crew (team) this CrewMember belongs too
	class ACrew* crew;

	// Character ID (index in crewMember array)
	int32 id;

	// Remaining amount of heatlh
	float health;
	
	//Speed this CrewMember moves.
	float Speed;
	
	//Standard amount of damage that attacks deal
	int32 damage;

	// Character's connection to the world 
	class AGridSpace* gridSpace;

	//	Weapon info
	class PowerUpEffect* gunEffect;

	//The team this CrewMember is on. 0 = Red team, 1 = blue team 
	int32 team;

	//The rifle for shooting in a cardinal direction
	class URifle* rifle;
	
	//The launcher for throwing a grenade onto a set of tiles
	class ULauncher* launcher;
	
};
