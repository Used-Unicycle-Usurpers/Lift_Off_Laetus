// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LaetusGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LIFT_OFF_LAETUS_API ULaetusGameInstance : public UGameInstance {
	GENERATED_BODY()

public:

	ULaetusGameInstance();

	UPROPERTY(BlueprintReadWrite)
		bool singleInput;
};
