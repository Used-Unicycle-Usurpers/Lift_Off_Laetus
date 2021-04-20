// Fill out your copyright notice in the Description page of Project Settings.


#include "CrewMember.h"
#include "Crew.h"
#include "../Weapons/Launcher.h"
#include "../Weapons/Rifle.h"
#include "../GameManagement/GridSpace.h"
#include "Components/InputComponent.h"

// Sets default values
ACrewMember::ACrewMember()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	//Taken from tutorial
	//SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>CharacterMeshAsset(TEXT("StaticMesh'/Game/Characters/CHAR_Pavo_Base.CHAR_Pavo_Base'"));
	Mesh->SetStaticMesh(CharacterMeshAsset.Object);

	Speed = 10.f;

	RootComponent = Mesh;
	
	rifle = CreateDefaultSubobject<URifle>("Rifle");
	rifle->mesh->SetVisibility(false);
	launcher = CreateDefaultSubobject<ULauncher>("Launcher");
	launcher->mesh->SetVisibility(false);
}

// For testing 

 void ACrewMember::SetTeam(int32 newTeam) {
	 if (newTeam) {
		 team = newTeam;
	 } else {
		 team = 3; //set team to green if nullptr
	 }
}



// Called when the game starts or when spawned
void ACrewMember::BeginPlay() {
	Super::BeginPlay();

	//TSubclassOf<AActor> ActorToSpawn;
	//AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(FVector(-80, 122, 0), FRotator(0, 0, 0));
	//UE_LOG(LogTemp, Warning, TEXT("Hello World!"));
}

// Called every frame
void ACrewMember::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACrewMember::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindKey(EKeys::A, IE_Released, this, &ACrewMember::Shove);
}

// Move to GridSpace 
void ACrewMember::MoveTo(AGridSpace * target) {
	// move to target grid space 
	// do we want it to return true or false to indicate 
	// if it was capable of completing the action 

	//will it decrease the crew's action bar?
}

// Shoot in a specific direction 
void ACrewMember::Shoot(FVector2D direction, bool useRifle) {
	// not sure if im understanding this method correctly
	if (useRifle) {
		rifle->fire(direction);
	} else {
		launcher->fire(direction);
	}
}

// Shove 
void ACrewMember::Shove() {
	UE_LOG(LogTemp, Warning, TEXT("hello"));
	// Not sure if we are going to give them the option of what to shove
}

// Take damage 
void ACrewMember::takeDamage(int32 damageTaken) {
	health -= damageTaken;

	if (health <= 0) {
		//destroy actor?
	}
		
}

/**
 * Set the AGridSpace this ACrewMember is currently standing on to the given
 * AGridSpace.
 * 
 * @param space a pointer to the new AGridSpace this ACrewMember is now
 *     standing on.
 */
void ACrewMember::setGridSpace(class AGridSpace* space) {
	gridSpace = space;
}

/**
 * Get the AGridSpace this ACrewMember is currently standing on. 
 * 
 * @returns a pointer to the AGridSpace this ACrewMember is currently 
 *     standing on.
 */
class AGridSpace* ACrewMember::getGridSpace() {
	return gridSpace;
}

