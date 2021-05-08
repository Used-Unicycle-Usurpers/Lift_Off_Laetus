// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasePowerUpEffectComponent.generated.h"


UCLASS( Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIFT_OFF_LAETUS_API UBasePowerUpEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBasePowerUpEffectComponent();
	
	// Number of turns before expiring (0 = never expires)
	UPROPERTY(EditAnywhere)
		uint8 maxTurnLifetime = 0;

	// Decrement the lifetime counter if it has a limit
	void DecrementLife();

	// Apply the effect
	virtual void ApplyEffect();

	// Remove the effect
	virtual void RemoveEffect();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Number of turns remaining in lifetime
	uint8 remainingTurns = 1;
};
