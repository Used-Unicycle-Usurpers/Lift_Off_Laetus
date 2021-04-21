// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "../GameManagement/Grid.h"


// Sets default values for this component's properties
UWeapon::UWeapon() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
}

// Called when the game starts
void UWeapon::BeginPlay() {
	Super::BeginPlay();

	TArray<AActor*> results;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrid::StaticClass(), results);
	if (results.Num() != 1) {
		UE_LOG(LogTemp, Warning, TEXT("Number of AGrid objects in the game is: %d"), results.Num());
	}

	grid = Cast<AGrid>(results[0]);
	
}

// Called every frame
void UWeapon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int UWeapon::fire(FVector2D direction) {
	return - 1;
}

