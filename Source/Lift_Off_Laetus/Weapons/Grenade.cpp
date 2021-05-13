// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../GameManagement/GridSpace.h"
#include "../GameManagement/Grid.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../PowerUps/PowerUpEffectData.h"
#include "../GameManagement/LaetusGameMode.h"
#include "../Controllers/CrewController.h"
#include "../Controllers/InputController.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"

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

	static ConstructorHelpers::FObjectFinder<UParticleSystem>explosion(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Gruntling/Bomber/Grenade_VFX.Grenade_VFX'"));
	grenadeExplosion = explosion.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue>sound(TEXT("SoundCue'/Game/Audio/Weapons/AUD_explosion01_Cue.AUD_explosion01_Cue'"));
	explosionSound = sound.Object;
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay() {
	Super::BeginPlay();
	current = 0;
	gameMode = Cast<ALaetusGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void AGrenade::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	float distance = FVector::Dist(GetActorLocation(), targetLocation);
	if (!exploded) {
		//If not reached the end of the path, keep going
		if (current < path.Num()) {
			//Still travelling on path, keep going
			SetActorLocation(path[current].Location);
			current++;
		}
		else {
			//Wait a few seconds, then let the grenade explode.
			exploded = true;
			FTimerHandle timerParams;
			GetWorld()->GetTimerManager().SetTimer(timerParams, this, &AGrenade::explode, 1.0f, false);
		}
	}
}

/**
 * Explode this grenade, damaging all players in its range.
 */
void AGrenade::explode() {
	//Reach end of path; at target. Damage nearby players.
	mesh->SetVisibility(false);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), grenadeExplosion, GetActorLocation());
	UGameplayStatics::PlaySound2D(GetWorld(), explosionSound);

	FVector2D gridLocation = targetSpace->getGridLocation();
	UPowerUpEffectData* effectToApply = owner->GetWeaponEffect();
	for (int row = gridLocation.X - 1; row <= gridLocation.X + 1; row++) {
		for (int column = gridLocation.Y - 1; column <= gridLocation.Y + 1; column++) {

			//Check this tile isn't a hole in the map.
			AGridSpace* space = grid->getTile(FVector2D(row, column));
			if (space) {
				//Valid space, temporarily highlight this tile as red
				//to show it's been hit
				space->SetToRedOnTimer();

				if (effectToApply != nullptr)
					effectToApply->ApplyTileEffect(space);

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

	if (effectToApply != nullptr)
		owner->ClearWeaponEffect();

	FTimerHandle timerParams;
	GetWorld()->GetTimerManager().SetTimer(timerParams, this, &AGrenade::destroySelf, 2.0f, false);
}

/**
 * Move the camera back the player, re-enable their input, and 
 * destroy this grenade.
 */
void AGrenade::destroySelf() {
	//Move camera back to thrower and re-enable input
	owner->getCrewController()->getInputController()->setStateToIdle();
	owner->getCrewController()->enableInputController();
	
	//Reset currently selected tile.
	owner->getCrewController()->getInputController()->currentlySelectedTile = owner->getGridSpace();
	
	//change turn if actionBar is 0
	if (gameMode->getABStatus() == 0) { gameMode->ChangeTurn(); }
	GetWorld()->DestroyActor(this);
}

