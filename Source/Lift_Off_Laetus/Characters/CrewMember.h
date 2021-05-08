// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Weapons/Rifle.h"
#include "../Weapons/Launcher.h"
#include "CrewMember.generated.h"

//Enum representing the four carnidal directions for player actions
UENUM()
enum Direction {
	InvalidDirection = -1,
	Left = 180,
	Right = 0,
	Up = 90,
	Down = 270
};

//Enum representing the different possible of rotations animation 
//an ACrewMember can do.
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
	void MoveTo(class AGridSpace * target, bool pushingCoreFragment);
	
	/**
	 * Shoot one of this ACrewMember's weapons in the given direction
	 */
	void Shoot(FVector2D target, bool useRifle);
	
	/**
	 * Shove the object that was in the AGridSpace this ACrewMember just moved
	 * into.
	 */
	void Shove(); // what if there is more than one shove option
	
	/**
	 * Reduce this ACrewMember's health by the given damage.
	 */
	void takeDamage(int32 damage); //excluded cause parameter 
	
	//The main character mesh for this ACrewMember
	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* skeletalMesh;
	
	//Default team color is red team's, so we need to save the 
	//other team's color in case they are assigned blue team
	UPROPERTY(EditAnywhere)
		class UMaterial* BlueTeamColor;

	UPROPERTY(EditAnywhere)
		class UMaterial* RedTeamColor;
	
	/**
	 * Set this ACrewMember's team to the given team
	 */
	void SetTeam(int32 team);

	/**
	 * Return the team this ACrewMember is a part of.
	 */
	int getTeam();

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
	float playThrowMontage();

	/**
	 * Play the shooting rifle montage.
	 */
	float playShootRifleMontage();

	/**
	 * Play the stumble montage (used when taking damage).
	 */
	float playStumbleMontage();

	/**
	 * Rotate this ACrewMember to the given direction, and play the appropriate
	 * animation while doing so.
	*/
	float rotateWithAnimation(Direction directionToFace);
	
	/**
	 * Play the given rotation animation.
	 */
	float playRotationMontage(RotationAnim type);

	/**
	 * Play the push montage (used when pushing a core fragment).
	 */
	float playPushMontage();

	//The current direction this ACrewMember is facing.
	Direction facingDirection;

	//Constant values representing the character rotations in world 
	//space for the four cardinal directions.
	const FRotator leftRotation = FRotator(0.f, 90.f, 0.f);
	const FRotator rightRotation = FRotator(0.f, 270.f, 0.f);
	const FRotator upRotation = FRotator(0.f, 180.f, 0.f);
	const FRotator downRotation = FRotator(0.f, 0.f, 0.f);

	/**
	 * Rotates this ACrewMember in world space to given direction.
	 */
	void rotateToDirection(Direction direction);

	/**
	 * Convert the given unit direction vector to the corresponding
	 * Direction enum value.
	 */
	Direction vectorToDirectionEnum(FVector2D direction);

	/**
	 * Returns the current value of the Speed variable.
	 *
	 * NOTE: Currently this variable is not used during movement, so it
	 * is only used to tell the animation blueprint that this ACrewMember
	 * is currently moving or not.
	 */
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

	/**
	 * Set the reference to the controller for the ACrew that this ACrewMember is a
	 * part of.
	 * NOTE: this is a reference to the controller that is possessing the ACrew. No
	 * controller is directly possessing any of the ACrewMembers.
	 */
	void setController(class ACrewController* newController);

	/**
	 * Get a refernce the controller that possess the ACrew this ACrewMember is a 
	 * part of.
	 */
	class ACrewController* getCrewController();

	bool needToRotate(FVector2D newDirection);

	//The camera that follows this crew member
	UPROPERTY(EditAnywhere)
		class UPowerUpEffectData* powerUp;

	class UPowerUpEffectData* GetWeaponEffect();
	void ClearWeaponEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//The controller for the ACrew that this ACrewMember belongs to.
	class ACrewController* controller;

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
	class UPowerUpEffectData* weaponEffect;

	//The team this CrewMember is on. 0 = Red team, 1 = blue team 
	int32 team;

	//The rifle for shooting in a cardinal direction
	class URifle* rifle;
	
	//The launcher for throwing a grenade onto a set of tiles
	class ULauncher* launcher;

	//Animation montages to play for using weapons
	class UAnimMontage* throwMontage;
	class UAnimMontage* shootRifleMontage;

	//Animations montages for rotating
	class UAnimMontage* turnLeftMontage;
	class UAnimMontage* turnRightMontage;
	class UAnimMontage* turnAroundMontage;

	//Animation montage for taking damage
	class UAnimMontage* stumbleMontage;

	//Animation montage for push core fragment
	class UAnimMontage* pushMontage;
	
	//A reference to the game map grid.
	AGrid* grid;

	//These variables are used to rotate before moving and then to 
	//animate movement forward.
	FVector newLocation;
	FVector moveIncrement;
	FTimerHandle moveTimerHandle;
	Direction directionToFaceEnum;
	class AGridSpace* targetLocation;
	
	/**
	 * Play the walking animation and inch the ACrewMember forward to
	 * the AGridSpace they're moving to.
	 */
	void moveForward();

	/**
	 * Called by the looping timer in moveForward. A single call to this
	 * function moves the ACrewMember forward by the amount in moveIncrement.
	 */
	void incrementMoveForward();

	//Helper functions used by rotateDirection
	void rotateUp();
	void rotateLeft();
	void rotateRight();
	void rotateDown();

	void die();

	bool pushing;
};
