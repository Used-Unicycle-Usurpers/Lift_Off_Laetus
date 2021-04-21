// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../GameManagement/GridSpace.h"
#include "../GameManagement/Grid.h"

// Sets default values
AGrenade::AGrenade() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("GrenadeMesh");
	static ConstructorHelpers::FObjectFinder<UStaticMesh>grenadeMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/grenade.grenade'"));
	mesh->SetStaticMesh(grenadeMesh.Object);
	mesh->SetWorldScale3D(FVector(50.f, 50.f, 50.f));
	SetRootComponent(mesh);

	collision = CreateDefaultSubobject<UCapsuleComponent>("GrenadeHitbox");
	movement = CreateDefaultSubobject<UProjectileMovementComponent>("Movement");
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay() {
	Super::BeginPlay();
	current = 0;
}

// Called every frame
void AGrenade::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	float distance = FVector::Dist(GetActorLocation(), targetLocation);
	UE_LOG(LogTemp, Warning, TEXT("Distance is: %f"), distance);
	
	//If not reached the end of the path, keep going
	if (current < path.Num() && distance > 10) {
		//Still travelling on path, keep going
		SetActorLocation(path[current].Location);
		current++;
	}else {
		//Reach end of path; at target. Damage nearby players.
		FVector2D gridLocation = targetSpace->getGridLocation();
		for (int row = gridLocation.X - 1; row <= gridLocation.X + 1; row++) {
			for (int column = gridLocation.Y - 1; column <= gridLocation.Y + 1; column++) {
				AGridSpace* s = grid->getTile(FVector2D(row, column));
				if (s) {
					s->SetToRed();
					ACrewMember* crewMember = s->getOccupant();
					if (crewMember) {
						//TODO: determine damage
						crewMember->takeDamage(10.0f);
					}
				}
			}
		}
	}
}

