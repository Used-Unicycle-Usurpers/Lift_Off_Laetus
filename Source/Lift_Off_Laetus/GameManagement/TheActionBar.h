// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LaetusGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "TheActionBar.generated.h"

/**
 * 
 */
UCLASS( Abstract )
class LIFT_OFF_LAETUS_API UTheActionBar : public UUserWidget
{
    GENERATED_BODY()

public:
    void UpdateActionBar(float newValue);

protected: 
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
        class UProgressBar* ActionBar = nullptr;

    

};
