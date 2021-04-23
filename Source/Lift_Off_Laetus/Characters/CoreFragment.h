// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreFragment.generated.h"

UCLASS()
class LIFT_OFF_LAETUS_API ACoreFragment : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoreFragment();

	//For visual debugging
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* mesh;

	void moveTo(class AGridSpace* target);

	void setGridSpace(class AGridSpace* space);
	class AGridSpace* getGridSpace();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class AGridSpace* gridSpace;

};
