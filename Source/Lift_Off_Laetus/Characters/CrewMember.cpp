// Fill out your copyright notice in the Description page of Project Settings.


#include "CrewMember.h"
#include "Crew.h"
#include "../GameManagement/GridSpace.h"

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
	
	//Set to blue team's (color 02) material 
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/Characters/Color_02_Blue.Color_02_Blue'"));
	//CrewColor = CreateDefaultSubobject<UMaterial>(TEXT("UMaterial'/Game/Characters/lambert1'"));
	BlueTeamolor = (UMaterial*)Material.Object;

	//physics 
	TInlineComponentArray<UPrimitiveComponent*> Components;
	GetComponents(Components);

	for (UPrimitiveComponent* Component : Components)
	{
		Component->SetSimulatePhysics(true);
	}
}

// For testing 

 void ACrewMember::SetTeam(int32 newTeam) {
	 if (newTeam) {
		 team = newTeam;
	 } 

	 //Change appearance based on team
	 //Red  - Color 01 - default
	 //Blue - Color 02 - update material 
	 if (team == 1) {
		// CrewColor = CreateDefaultSubobject<UMaterial>(TEXT("UMaterial'/Game/Characters/lambert1_2'"));
		Mesh->SetMaterial(0, BlueTeamolor);
	 }
	 
	 
}



// Called when the game starts or when spawned
void ACrewMember::BeginPlay()
{
	Super::BeginPlay();

	//TSubclassOf<AActor> ActorToSpawn;
	//AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(FVector(-80, 122, 0), FRotator(0, 0, 0));
	//UE_LOG(LogTemp, Warning, TEXT("Hello World!"));
}

// Called every frame
void ACrewMember::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACrewMember::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Move to GridSpace 
void ACrewMember::MoveTo(AGridSpace * target) {
	// move to target grid space 
	// do we want it to return true or false to indicate 
	// if it was capable of completing the action 

	//will it decrease the crew's action bar?
}

// Shoot in a specific direction 
void ACrewMember::Shoot(FVector direction) {
	// not sure if im understanding this method correctly 
}

// Shove 
void ACrewMember::Shove() {
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

