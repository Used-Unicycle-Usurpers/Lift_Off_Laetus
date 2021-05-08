// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterAnimDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class LIFT_OFF_LAETUS_API UCharacterAnimDataAsset : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class USkeletalMesh* skeletalMesh;

	UPROPERTY(EditAnywhere)
		class TSubclassOf<class UAnimInstance> animBP;

	//Animation montages to play for using weapons
	UPROPERTY(EditAnywhere)
		class UAnimMontage* throwMontage;
	
	UPROPERTY(EditAnywhere)
		float throwMontageDelay;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* shootRifleMontage;

	//Animations montages for rotating
	UPROPERTY(EditAnywhere)
		class UAnimMontage* turnLeftMontage;
	
	UPROPERTY(EditAnywhere)
		class UAnimMontage* turnRightMontage;
	
	UPROPERTY(EditAnywhere)
		class UAnimMontage* turnAroundMontage;

	//Animation montage for taking damage
	UPROPERTY(EditAnywhere)
		class UAnimMontage* stumbleMontage;

	//Animation montage for push core fragment
	UPROPERTY(EditAnywhere)
		class UAnimMontage* pushMontage;
};
