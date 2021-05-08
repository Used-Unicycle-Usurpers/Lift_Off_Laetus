// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../GameManagement/GridSpace.h"
#include "../GameManagement/Grid.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../Controllers/CrewController.h"

// Sets default values
AGrenade::AGrenade() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("GrenadeMesh");
	static ConstructorHelpers::FObjectFinder<UStaticMesh>grenadeMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/grenade.grenade'"));
	mesh->SetStaticMesh(grenadeMesh.Object);
	mesh->SetWorldScale3D(FVector(50.f, 50.f, 50.f));
	SetRootComponent(mesh);

	movement = CreateDefaultSubobject<UProjectileMovementComponent>("Movement");

	cameraArm = CreateDefaultSubobject<USpringArmComponent>("CameraSpringArm");
	cameraArm->SetupAttachment(mesh);
	cameraArm->SetAbsolute(false, true, false);
	cameraArm->SetWorldRotation(FRotator(320.f, 270.f, 0.f));
	cameraArm->TargetArmLength = 1150.f;
	cameraArm->bDoCollisionTest = false;

	camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	camera->AttachToComponent(cameraArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);
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
				
				//Check this tile isn't a hole in the map.
				AGridSpace* space = grid->getTile(FVector2D(row, column));
				if (space) {
					//Valid space, temporarily highlight this tile as red
					//to show it's been hit
					space->SetToRedOnTimer();
					
					//Check each tile for a player to damage
					AActor* occupant = space->getOccupant();
					if (occupant) {
						ACrewMember* crewMember = Cast<ACrewMember>(occupant);
						if (crewMember) {
							//TODO: determine damage
							crewMember->takeDamage(1.0f);
						}
					}
				}
			}
		}
		
		//Wait a few seconds, then let the grenade explode.
		FTimerHandle timerParams;
		GetWorld()->GetTimerManager().SetTimer(timerParams, this, &AGrenade::destroySelf, 2.0f, false);
	}
}

/**
 * Explode this grenade, and move the camera back the player.
 */
void AGrenade::destroySelf() {
	owner->getCrewController()->moveCameraSmoothly(owner);
	owner->getCrewController()->enableInputController();
	//owner->getCrewController()->setStateToIdle();
	GetWorld()->DestroyActor(this);
}

