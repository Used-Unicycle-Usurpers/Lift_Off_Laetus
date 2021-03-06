// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreFragmentReceiver.h"
#include "Lift_Off_Laetus/Characters/Crew.h"
#include "Lift_Off_Laetus/Characters/CoreFragment.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ACoreFragmentReceiver::ACoreFragmentReceiver() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>receiverMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/ENV_Tile/ENV_Goal_Tile.ENV_Goal_Tile'"));
	mesh->SetStaticMesh(receiverMesh.Object);

	static ConstructorHelpers::FObjectFinder<UMaterial>material0(TEXT("Material'/Game/Geometry/Meshes/ENV_Tile/Goal_Metalq.Goal_Metalq'"));
	static ConstructorHelpers::FObjectFinder<UMaterial>material1(TEXT("Material'/Game/Geometry/Meshes/ENV_Tile/Goal_Glow.Goal_Glow' "));
	mesh->SetMaterial(0, material0.Object);
	mesh->SetMaterial(1, material1.Object);
	
	mesh->SetWorldScale3D(FVector(1.f, 1.f, 0.5f));
	collision->SetRelativeScale3D(FVector(3.f, 3.f, 2.f));
	overlayMesh->SetRelativeScale3D(FVector(2.f, 2.f, 1.f));
	overlayMesh->SetRelativeLocation(FVector(0.f, 0.f, 3.f));

	static ConstructorHelpers::FObjectFinder<USoundCue>sound(TEXT("SoundCue'/Game/Audio/AUD_score_point_Cue.AUD_score_point_Cue'"));
	scoreSound = sound.Object;
}

void ACoreFragmentReceiver::BeginPlay() {
	Super::BeginPlay();
	// TODO: Replace this scaling with something better
	//mesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 0.1f));
}

void ACoreFragmentReceiver::SetCrew(ACrew* crew) {
	ownedBy = crew;
}

void ACoreFragmentReceiver::OnCoreFragmentReceived(ACoreFragment* core) {
	//Play the score sound
	UGameplayStatics::PlaySound2D(GetWorld(), scoreSound);

	// Increase core count by 1
	if (ownedBy) {
		ownedBy->incrementCores();
	}else {
		UE_LOG(LogTemp, Warning, TEXT("ownedBy was null for ACoreFragmentReceiver: %s"), *GetName());
	}

	// Destroy the fragment core
	GetWorld()->DestroyActor(core);
}