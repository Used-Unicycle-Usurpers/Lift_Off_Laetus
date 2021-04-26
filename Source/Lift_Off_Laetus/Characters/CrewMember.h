// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Weapons/Rifle.h"
#include "../Weapons/Launcher.h"
#include "../PowerUps/PowerUpEffect.h"
#include "CrewMember.generated.h"

UENUM()
enum Direction {
	InvalidDirection = -1,
	Left = 180,
	Right = 0,
	Up = 90,
	Down = 270
};

UENUM()
enum RotationAnim {
	TurnLeft,
	TurnRight,
	TurnAround
};


UCLASS()
class LIFT_OFF_LAETUS_API ACrewMember : public APawn {
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
	
	/*
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Mesh;
	*/

	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* skeletalMesh;
	
	//Default team color is red team's, so we need to save the 
	//other team's color in case they are assigned blue team
	UPROPERTY(EditAnywhere)
		class UMaterial* BlueTeamColor;

	UPROPERTY(EditAnywhere)
		class UMaterial* RedTeamColor;
	
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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Speed this CrewMember moves.
	UPROPERTY(EditAnywhere)
		float Speed;

	/**
	 * Play the grenade throwing montage.
	 */
	void playThrowMontage();

	/**
	 * Play the shooting rifle montage.
	 */
	void playShootRifleMontage();
	
	/**
	 * Rotate this ACrewMember to the given direction, and play the appropriate
	 * animation while doing so.
	*/
	float rotateWithAnimation(Direction directionToFace);
	
	/**
	 * Play the given rotation animation.
	 */
	float playRotationMontage(RotationAnim type);

	int playStumbleMontage();

	Direction facingDirection;

	const FRotator leftRotation = FRotator(0.f, 90.f, 0.f);
	const FRotator rightRotation = FRotator(0.f, 270.f, 0.f);
	const FRotator upRotation = FRotator(0.f, 180.f, 0.f);
	const FRotator downRotation = FRotator(0.f, 0.f, 0.f);

	void rotateToDirection(Direction direction);
	void rotateUp();
	void rotateLeft();
	void rotateRight();
	void rotateDown();

	/**
	 * Convert the given unit direction vector to the corresponding
	 * Direction enum value.
	 */
	Direction vectorToDirectionEnum(FVector2D direction);

	// Below is supposed to be the hitbox, needs testing
	//watch video to see what he says 

	UFUNCTION(BlueprintCallable)
		float getSpeed();

	UFUNCTION(BlueprintCallable)
		void onRotationAnimationEnd(UAnimMontage* montage, bool wasInteruppted);

	//The sprint arm that holds the camera
	UPROPERTY(EditAnywhere)
		class USpringArmComponent* cameraArm;

	//The camera that follows this crew member
	UPROPERTY(EditAnywhere)
		class UCameraComponent* camera;

	int getTeam();

	void setController(class ACrewController* newController);

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

	//Animation montages to play for character actions
	class UAnimMontage* throwMontage;
	class UAnimMontage* shootRifleMontage;

	class UAnimMontage* turnLeftMontage;
	class UAnimMontage* turnRightMontage;
	class UAnimMontage* turnAroundMontage;

	class UAnimMontage* stumbleMontage;
	
	//A reference to the game map grid.
	AGrid* grid;

	//Used to rotate before moving and then to animate movement forward
	FVector newLocation;
	FVector moveIncrement;
	FTimerHandle moveTimerHandle;
	Direction directionToFaceEnum;
	class AGridSpace* targetLocation;
	void moveForward();
	void incrementMoveForward();

	class ACrewController* controller;
};
