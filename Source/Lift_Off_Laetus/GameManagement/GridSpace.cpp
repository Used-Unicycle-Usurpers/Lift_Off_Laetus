// Fill out your copyright notice in the Description page of Project Settings.

#include "GridSpace.h"
#include "../Characters/CoreFragment.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

static UMaterial* regularMaterial;
static UMaterial* redMaterial;
static UMaterial* blueMaterial;
static UMaterial* greenMaterial;

// Sets default values
AGridSpace::AGridSpace(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//collision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	//Get the color materials for visual debugging of the grid spaces
	static ConstructorHelpers::FObjectFinder<UMaterial>regular(TEXT("Material'/Game/Geometry/Meshes/ENV_Tile/ENV_tile_mat.ENV_tile_mat'"));
	regularMaterial = regular.Object;
	static ConstructorHelpers::FObjectFinder<UMaterial>red(TEXT("Material'/Game/RedMaterial.RedMaterial'"));
	redMaterial = red.Object;
	static ConstructorHelpers::FObjectFinder<UMaterial>blue(TEXT("Material'/Game/BlueMaterial.BlueMaterial'"));
	blueMaterial = blue.Object;
	static ConstructorHelpers::FObjectFinder<UMaterial>green(TEXT("Material'/Game/GreenMaterial.GreenMaterial'"));
	greenMaterial = green.Object;

	//Get mesh for visual debugging
	static ConstructorHelpers::FObjectFinder<UStaticMesh>CubeMeshAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));
	mesh->SetStaticMesh(CubeMeshAsset.Object);
	mesh->SetRelativeScale3D(FVector(2.f, 2.f, 0.1f));
	SetToRegularMaterial();

	/*
	//Set up box collision component, which will be used to keep track of the current
	//occupant of this AGridSpace, if any.
	collision->SetRelativeScale3D(FVector(1.55f, 1.55f, 5.f));
	collision->SetHiddenInGame(false);//Only visible for debugging
	collision->AttachToComponent(mesh, FAttachmentTransformRules::KeepRelativeTransform);
	collision->SetGenerateOverlapEvents(true);
	collision->OnComponentBeginOverlap.AddDynamic(this, &AGridSpace::OnEnterGridSpace);
	collision->OnComponentEndOverlap.AddDynamic(this, &AGridSpace::OnExitGridSpace);
	collision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	*/

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
void AGridSpace::BeginPlay(){
	Super::BeginPlay();
}

/**
 * Called when a new actor walks onto this AGridSpace. If it was a CrewMember, set it as 
 * the new occupant of this AGridSpace if is a valid actor (i.e. not pending kill and not 
 * NULL).
 * 
 * @param OverlappedComponent the component of this AGridSpace that overlapped with 
 *     OtherComp
 * @param OtherActor the actor that started overlapping with this AGridSpace
 * @param OtherComponent the component of OtherActor that started overlapping with 
 *	   this AGridSpace
 * @param OtherBodyIndex index of the mesh that was overlapped
 * @param bFromSweep true if occured by a sweep, false otherwise
 * @param SweepResult the FHitResult containing the details about the overlap
 */
void AGridSpace::OnEnterGridSpace(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ACrewMember* newOccupant = Cast<ACrewMember>(OtherActor);
	if (IsValid(newOccupant)) {
		setOccupant(newOccupant);
	}
}

/**
 * Called when the current occupant of this AGridSpace walks off. Occupant
 * is set to nullptr.
 * 
 * @param OverlappedComponent the component of this AGridSpace that has now stopped 
 *     overlapping with OtherComp
 * @param OtherActor the actor that stopped overlapping with this AGridSpace
 * @param OtherComponent the component of OtherActor that has now stopped overlapping
 *     with this AGridSpace
 * @param OtherBodyIndex
 * @param bFromSweep
 */
void AGridSpace::OnExitGridSpace(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	setOccupant(nullptr);
}

void AGridSpace::SetToRegularMaterial() {
	mesh->SetMaterial(0, regularMaterial);
}

/**
 * For visual debugging, set tile color
 */
void AGridSpace::SetToRed() {
	mesh->SetMaterial(0, redMaterial);
}

void AGridSpace::SetToRedOnTimer() {
	SetToRed();
	FTimerHandle f;
	GetWorld()->GetTimerManager().SetTimer(f, this, &AGridSpace::SetToRegularMaterial, 2.0f, false);
}

void AGridSpace::SetToBlue() {
	mesh->SetMaterial(0, blueMaterial);
}

void AGridSpace::SetToGreen() {
	mesh->SetMaterial(0, greenMaterial);
}

/**
 * Set the occupant of this AGridSpace to newOccupant.
 * @param newOccupant the new occupant of this AGridSpace. If NULL, then
 *     this AGridSpace will be considered as having no occupant.
 * 
 */
void AGridSpace::setOccupant(AActor* newOccupant) {
	occupant = newOccupant;
}

/**
 * Returns the current occupant of this AGrid space.
 * @return pointer to the crewMember currently occupying this AGridSpace,
 *     nullptr if there is no occupant
 */
AActor* AGridSpace::getOccupant() {
	return occupant;
}

/**
* Return if this AGridSpace is currently occupied.
* 
* @return true if this AGridSpace is occupied but a valid CrewMember 
*     (i.e. not pending kill and not NULL), false otherwise.
*/
bool AGridSpace::isOccupied() {
	return (occupant != nullptr) && IsValid(occupant);
}

bool AGridSpace::containsFragment() {
	if (isOccupied()) {
		ACoreFragment* fragment = Cast<ACoreFragment>(occupant);
		return IsValid(fragment);
	}
	return false;
}

/**
 * Set the row and column of this AGridSpace to the given row and column.
 * 
 * @param row the row number for this AGridSpace
 * @param column the column number for this AGridSpace
 */
void AGridSpace::setGridLocation(int32 row, int32 column) {
	gridLocation = FVector2D(row, column);
}

/**
 * Returns the row and column of this AGridSpace.
 * 
 * @return a FVector2D, where the X component is the row number and the
 *     Y component in the column number.
 */
FVector2D AGridSpace::getGridLocation() {
	return gridLocation;
}

/**
 * Set the AHarvestSource occupants of this tile can harvest from, to the given
 * AHarvestSource. For visuall debugging, the AGridSpace will turn red if it now
 * has a refernce to an ASlimeTree, blue if Rock, and green if Shrub.
 * 
 * @param newSource a pointer to an AHarvestSource that this AGridSpace will now
 *     have access to.
 */
void AGridSpace::setHarvestSource(AHarvestSource* newSource) {
	harvestSource = newSource;
	/*
	switch (harvestSource->getHarvestSourceType()) {
	case SlimeTree:
		SetToRed();
		break;
	case Rock:
		SetToBlue();
		break;
	case Shrub:
		SetToGreen();
		break;
	}
	*/
}

/**
 * Returns the AHarvestSource that this tile allows the occupant to harvest
 * from during their turn.
 *
 * @return a pointer to the AHarvestSource that this AGridSpace as access to.
 */
AHarvestSource* AGridSpace::getHarvestSource() {
	return harvestSource;
}

// Called every frame
void AGridSpace::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}


/**
* Set the "extra" movement cost for entities moving through this space.
* 
* @param cost - the new movement cost
*/
void AGridSpace::SetExtraMoveCost(int cost) {
	extraMoveCost = cost;
}

/**
* Gets the "extra" movement cost value on this space.
* 
* @return the cost, as an integer.
*/
int32 AGridSpace::GetExtraMoveCost() {
	return extraMoveCost;
}

/**
* Resets the "extra" movement cost on this space to zero.
*/
void AGridSpace::ResetExtraMoveCost() {
	extraMoveCost = 0;
}
