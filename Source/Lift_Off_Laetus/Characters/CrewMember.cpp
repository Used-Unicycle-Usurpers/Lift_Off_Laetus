// Fill out your copyright notice in the Description page of Project Settings.


#include "CrewMember.h"
#include "Crew.h"
#include "../Weapons/Launcher.h"
#include "../Weapons/Rifle.h"
#include "../GameManagement/GridSpace.h"
#include "Components/InputComponent.h"

// Sets default values
ACrewMember::ACrewMember() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	static ConstructorHelpers::FObjectFinder<UStaticMesh>CharacterMeshAsset(TEXT("StaticMesh'/Game/Characters/CHAR_Pavo_Base.CHAR_Pavo_Base'"));
	Mesh->SetStaticMesh(CharacterMeshAsset.Object);

	Speed = 10.f;

	RootComponent = Mesh;
	
	//Create and attach the rifle and grenade
	rifle = CreateDefaultSubobject<URifle>("Rifle");
	rifle->mesh->SetVisibility(false);
	rifle->mesh->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform, "GunSocket");
	rifle->mesh->SetRelativeLocation(FVector(0, 0, 0));
	
	launcher = CreateDefaultSubobject<ULauncher>("Launcher");
	launcher->mesh->SetVisibility(false);
	launcher->mesh->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform, "GunSocket");
	launcher->mesh->SetRelativeLocation(FVector(0, 0, 0));

	//Set to blue team's (color 02) material 
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/Characters/Color_02_Blue.Color_02_Blue'"));
	BlueTeamColor = (UMaterial*)Material.Object;

	//physics 
	TInlineComponentArray<UPrimitiveComponent*> Components;
	GetComponents(Components);

	for (UPrimitiveComponent* Component : Components) {
		Component->SetSimulatePhysics(true);
	}
}

/**
 * Set this ACrewMember's team to the given team
 * 
 * @param newTeam the new team to assign to this ACrewMember.
 *     0 for red team, 1 for blue team.
 */
 void ACrewMember::SetTeam(int32 newTeam) {
	 if (newTeam) {
		 team = newTeam;
	 } 

	 //Change appearance based on team
	 //Red  - Color 01 - default
	 //Blue - Color 02 - update material 
	 if (team == 1) {
		// CrewColor = CreateDefaultSubobject<UMaterial>(TEXT("UMaterial'/Game/Characters/lambert1_2'"));
		Mesh->SetMaterial(0, BlueTeamColor);
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

/**
 * Move this ACrewMember to the given AGridSpace
 * 
 * @param target a pointer to the AGridSpace to move this
 *     ACrewMember to.
 */
void ACrewMember::MoveTo(AGridSpace * target) {
	// move to target grid space 
	// do we want it to return true or false to indicate 
	// if it was capable of completing the action 

	//will it decrease the crew's action bar?
}

/**
 * Shoot one of this ACrewMember's weapons in the given direction
 * 
 * @param direction the cardinal direction (unit vector) to shoot the 
 *     rifle in (if useRifle is true) the (row, column) of the space 
 *     to throw a grenade to (if useRifle is false).
 * @param useRifle true if shooting with a rifle, false  if throwing a 
 *     grenade.
 */
void ACrewMember::Shoot(FVector2D direction, bool useRifle) {
	if (useRifle) {
		rifle->fire(direction);
	} else {
		launcher->fire(direction);
	}
}

/**
 * Shove the object that was in the AGridSpace this ACrewMember just moved 
 * into.
 */
void ACrewMember::Shove() {
	UE_LOG(LogTemp, Warning, TEXT("hello"));
	// Not sure if we are going to give them the option of what to shove
}

/**
 * Reduce this ACrewMember's health by the given damage.
 * 
 * @param damageTaken the amount of damage to reduce this ACrewMember's 
 *     health by
 */
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
AGridSpace* ACrewMember::getGridSpace() {
	return gridSpace;
}

/**
 * Returns the ACrew (team) this ACrewMember is a part of.
 * 
 * @return a pointer to the ACrew this ACrewMember is a part of.
 */
ACrew* ACrewMember::getCrew() {
	return crew;
}