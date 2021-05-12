// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridSpace.h"
#include "CoreFragmentReceiver.generated.h"

class ACrew;
class ACoreFragment;

/**
 * 
 */
UCLASS()
class LIFT_OFF_LAETUS_API ACoreFragmentReceiver : public AGridSpace
{
	GENERATED_BODY()

public:
	ACoreFragmentReceiver();
	void SetCrew(ACrew* crew);
	void OnCoreFragmentReceived(ACoreFragment* core);
protected:
	virtual void BeginPlay() override;
private:
	ACrew* ownedBy;
};
