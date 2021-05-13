// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Weapons/Rifle.h"
#include "../Weapons/Launcher.h"
#include "../GameManagement/GameEnums.h"
#include "CrewMember.generated.h"

UCLASS()
class LIFT_OFF_LAETUS_API ACrewMember : public APawn {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACrewMember();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * Loads up the CharacterAnimDataAsset and assigns the corresponding
	 * meshes, materials, and animations beased on the provided character
	 * and team.
	 */
	void setMeshAnimData(FCharacter character, Team playerTeam);

	/**
	 * Set this ACrewMember's team to the given team
	 */
	void SetTeam(Team team);

	/**
	 * Return the team this ACrewMember is a part of.
	 */
	Team getTeam();

	/**
	 * Set the index of this ACrewMember in its team.
	 */
	void setID(int32 newidNum);

	/**
	 * Return the index of this ACrewMember in its team.
	 */
	int getID();

	/**
	 * Rotate this ACrewMember to the target direction and begin moving them forward
	 * until they have reached the target AGridSpace.
	 */
	void MoveTo(class AGridSpace * target, bool pushingCoreFragment);
	
	/**
	 * Shoot one of this ACrewMember's weapons in the given direction
	 */
	void Shoot(FVector2D target, bool useRifle);

	/**
	 * Rotate the player in the given direction and then punch at the ACrewMember
	 * in the adjacent AGridSpace in that given direction, if there is indeed an
	 * ACrewMember ther.
	 */
	void Punch(FVector2D direction);
	
	/**
	 * Shove the object that was in the AGridSpace this ACrewMember just moved
	 * into.
	 */
	void Shove(); // what if there is more than one shove option
	
	/**
	 * Reduce this ACrewMember's health by the given damage.
	 */
	void takeDamage(int32 damage); //excluded cause parameter 
	
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
	
	/**
	 * Play the grenade throwing montage.
	 */
	float playThrowMontage();

	/**
	 * Play the shooting rifle montage.
	 */
	float playShootRifleMontage();

	/**
	 * Play the punch montage.
	 */
	float playPunchMontage();

	/**
	 * Play the take damage montage.
	 */
	float playTakeDamageMontage();

	/**
	 * Play the death montage.
	 */
	float playDeathMontage();

	/**
	 * Play the stumble montage.
	 */
	float playStumbleMontage();

	/**
	 * Play the push montage (used when pushing a core fragment).
	 */
	float playPushMontage();

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
	 * Convert the given unit direction vector to the corresponding
	 * Direction enum value.
	 */
	Direction vectorToDirectionEnum(FVector2D direction);

	/**
	 * Rotates this ACrewMember in world space to given direction.
	 */
	void rotateToDirection(Direction direction);

	UFUNCTION(BlueprintCallable)
		void onRotationAnimationEnd(UAnimMontage* montage, bool wasInteruppted);

	bool needToRotate(FVector2D newDirection);

	/**
	 * Returns the current value of the Speed variable.
	 *
	 * NOTE: Currently this variable is not used during movement, so it
	 * is only used to tell the animation blueprint that this ACrewMember
	 * is currently moving or not.
	 */
	UFUNCTION(BlueprintCallable)
		float getSpeed();

	/**
	 * Set the reference to the controller for the ACrew that this ACrewMember is a
	 * part of.
	 * NOTE: this is a reference to the controller that is possessing the ACrew. No
	 * controller is directly possessing any of the ACrewMembers.
	 */
	void setCrewController(class ACrewController* newController);

	/**
	 * Get a refernce the controller that possess the ACrew this ACrewMember is a
	 * part of.
	 */
	class ACrewController* getCrewController();

	//The current direction this ACrewMember is facing.
	Direction facingDirection;

	//Constant values representing the character rotations in world 
	//space for the four cardinal directions.
	const FRotator leftRotation = FRotator(0.f, 90.f, 0.f);
	const FRotator rightRotation = FRotator(0.f, 270.f, 0.f);
	const FRotator upRotation = FRotator(0.f, 180.f, 0.f);
	const FRotator downRotation = FRotator(0.f, 0.f, 0.f);

	//The main character mesh for this ACrewMember
	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* skeletalMesh;

	//Default team color is red team's, so we need to save the 
	//other team's color in case they are assigned blue team
	UPROPERTY(EditAnywhere)
		class UMaterial* BlueTeamColor;

	UPROPERTY(EditAnywhere)
		class UMaterial* RedTeamColor;

	//Speed this CrewMember moves.
	UPROPERTY(EditAnywhere)
		float Speed;

	//The sprint arm that holds the camera
	UPROPERTY(EditAnywhere)
		class USpringArmComponent* cameraArm;

	//The camera that follows this crew member
	UPROPERTY(EditAnywhere)
		class UCameraComponent* camera;
        
    UPROPERTY(EditAnywhere)
		class UPowerUpEffectData* powerUp;
        
    class UPowerUpEffectData* GetWeaponEffect();
	void ClearWeaponEffect();

	//Amount of time, in seconds, into the throw grenade animation when
	//the ACrewMember actually throws the grenade.
	float throwMontageDelay;

	//The maximum amout of health an ACrewMember can have.
	const float maxHealth = 10.0f;

	/**
	 * Returns the current amount of health this ACrewMember has.
	 */
	float getCurrentHealth();

	class ALaetusGameMode* getGameMode();

private:
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
	
	/**
	 * Play the punch montage, and set timers to deal damage and
	 * re-enable input.
	 */
	void punchAtDirection();

	/**
	 * Deal punch damage to the occupant of targetLocation if
	 * they are an ACrewMember.
	 */
	void dealPunchDamage();

	/**
	 * Re-enables input after the punch has occurred.
	 */
	void enableInputAfterPunch();

	/**
	 * Called when this ACrewMember has died (i.e. health <= 0). Plays
	 * the death montage and calls respawn once montage has ended.
	 */
	void die();

	/**
	 * Find a valid respawn point, and move this ACrewMember there. Also resets
	 * the corresponding AInputController's currentlySelectedTile to this new
	 * location.
	 */
	void respawn();

	/**
	 * Rotate this ACrewMember to face upward i.e. away from the screen.
	 */
	void rotateUp();

	/**
	 * Rotate this ACrewMember to face left.
	 */
	void rotateLeft();

	/**
	 * Rotate this ACrewMember to face right.
	 */
	void rotateRight();

	/**
	 * Rotate this ACrewMember to face downward i.e. towards the screen.
	 */
	void rotateDown();

	//A reference to the game mode for quick access.
	class ALaetusGameMode* gameMode;

	//The controller for the ACrew that this ACrewMember belongs to.
	class ACrewController* controller;

	// The Crew (team) this CrewMember belongs too
	class ACrew* crew;

	// Character ID (index in crewMember array)
	int32 idNum;

	// Remaining amount of heatlh
	float health;
	
	//Standard amount of damage that attacks deal
	int32 damage;

	// Character's connection to the world 
	class AGridSpace* gridSpace;

	//	Weapon info
	class UPowerUpEffectData* weaponEffect;

	//The team this CrewMember is on 
	Team team;

	//The rifle for shooting in a cardinal direction
	class URifle* rifle;
	
	//The launcher for throwing a grenade onto a set of tiles
	class ULauncher* launcher;

	//Animation montages to play for using weapons
	class UAnimMontage* throwMontage;
	class UAnimMontage* shootRifleMontage;
	class UAnimMontage* punchMontage;

	//Animation montage for taking damage
	class UAnimMontage* takeDamageMontage;

	//Animation montage for dying.
	class UAnimMontage* deathMontage;

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
	//The (row, column) this ACrewMember is moving to.
	FVector newLocation;

	//The number of small increments to this ACrewMember will move 
	//when moving from their current location to newLocation.
	int numIncrements;

	//The number of increments left before this ACrewMember has 
	//reached newLocation.
	int incrementsLeft;

	//Vector representing the amount this ACrewMember moves during 
	//each increment, until it ultimately reaches newLocation.
	FVector moveIncrement;

	//FTimerHandle for the looping time that moves the ACrewMember from
	//oldLocation to newLocation.
	FTimerHandle moveTimerHandle;

	//The direction this ACrewMember is going to face after they have
	//finished rotating.
	Direction directionToFaceEnum;

	//The AGridSpace this ACrewMember is going to move to.
	class AGridSpace* targetLocation;

	//True if this ACrewMember is going to be pushing an ACoreFragment
	//when they start moving, false otherwise.
	bool pushing;

	//Sound effect for when the ACrewMember dies.
	class USoundCue* deathSound;

	//Sound effect for when the ACrewMember take damage.
	class USoundCue* takeDamageSound;

	//Sound effect for when the ACrewMember slips on a tile.
	class USoundCue* slipSound;

	//Sound effect for when the ACrewMember is moving.
	class USoundCue* footstepSound;

	class USoundCue* punchSound;
};
