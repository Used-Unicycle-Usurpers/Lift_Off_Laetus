// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterAnimDataAsset.generated.h"

/**
 * Data asset containing all data pertaining to character meshes, materials and animations.
 */
UCLASS()
class LIFT_OFF_LAETUS_API UCharacterAnimDataAsset : public UDataAsset {
	GENERATED_BODY()

public:
	//The skeletal mesh to use for the ACrewMember.
	UPROPERTY(EditAnywhere)
		class USkeletalMesh* skeletalMesh;

	//The Animation Blueprint for the ACrewMember
	UPROPERTY(EditAnywhere)
		class TSubclassOf<class UAnimInstance> animBP;

	//Animation montages to play for throwing grenades.
	UPROPERTY(EditAnywhere)
		class UAnimMontage* throwMontage;
	
	//The amount of time to wait after the throwMontage starts 
	//before you actually throw the grenade.
	UPROPERTY(EditAnywhere)
		float throwMontageDelay;

	//The animation montage for shooting the rifle.
	UPROPERTY(EditAnywhere)
		class UAnimMontage* shootRifleMontage;

	//The animation montage for punching.
	UPROPERTY(EditAnywhere)
		class UAnimMontage* punchMontage;

	//Animation montage for rotating 90 degrees to the left.
	UPROPERTY(EditAnywhere)
		class UAnimMontage* turnLeftMontage;

	//Animation montage for rotating 90 degrees to the right.
	UPROPERTY(EditAnywhere)
		class UAnimMontage* turnRightMontage;
	
	//Animation montage for rotating 180 degrees.
	UPROPERTY(EditAnywhere)
		class UAnimMontage* turnAroundMontage;

	//Animation montage for stumbling / falling over.
	UPROPERTY(EditAnywhere)
		class UAnimMontage* stumbleMontage;

	//Animation montage for taking damage
	UPROPERTY(EditAnywhere)
		class UAnimMontage* takeDamageMontage;

	//Animation montage for taking damage
	UPROPERTY(EditAnywhere)
		class UAnimMontage* deathMontage;

	//Animation montage for pushing a core fragment
	UPROPERTY(EditAnywhere)
		class UAnimMontage* pushMontage;

	//Main material for characters on the red team
	UPROPERTY(EditAnywhere)
		class UMaterial* redTeamMainMaterial;

	//Main material for characters on the blue team
	UPROPERTY(EditAnywhere)
		class UMaterial* blueTeamMainMaterial;

	//Armor material for characters on the red team
	UPROPERTY(EditAnywhere)
		class UMaterial* redTeamArmorMaterial;

	//Armor material for characters on the blue team
	UPROPERTY(EditAnywhere)
		class UMaterial* blueTeamArmorMaterial;

	//Material for the body of the rifle if the ACrewMember 
	//is on the red team.
	UPROPERTY(EditAnywhere)
		class UMaterial* redGunBodyMaterial;

	//Material for the sight of the rifle if the ACrewMember 
	//is on the red team.
	UPROPERTY(EditAnywhere)
		class UMaterial* redGunSightMaterial;

	//Material for the body of the rifle if the ACrewMember 
	//is on the blue team.
	UPROPERTY(EditAnywhere)
		class UMaterial* blueGunBodyMaterial;

	//Material for the sight of the rifle if the ACrewMember 
	//is on the blue team.
	UPROPERTY(EditAnywhere)
		class UMaterial* blueGunSightMaterial;
	
	//Sound effect for when the ACrewMember dies.
	UPROPERTY(EditAnywhere)
		class USoundCue* deathSound;

	//Sound effect for when the ACrewMember take damage.
	UPROPERTY(EditAnywhere)
		class USoundCue* takeDamageSound;
	
	//Sound effect for when the ACrewMember slips on a tile.
	UPROPERTY(EditAnywhere)
		class USoundCue* slipSound;

	//Sound effect for when the ACrewMember is moving.
	UPROPERTY(EditAnywhere)
		class USoundCue* footstepSound;
};
