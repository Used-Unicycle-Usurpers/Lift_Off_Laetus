// Fill out your copyright notice in the Description page of Project Settings.


#include "CrewMember.h"
#include "Crew.h"
#include "../Weapons/Launcher.h"
#include "../Weapons/Rifle.h"
#include "../GameManagement/GridSpace.h"
#include "../GameManagement/Grid.h"
#include "../GameManagement/LaetusGameMode.h"
#include "Components/InputComponent.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Controllers/CrewController.h"

// Sets default values
ACrewMember::ACrewMember() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	//static ConstructorHelpers::FObjectFinder<UStaticMesh>CharacterMeshAsset(TEXT("StaticMesh'/Game/Characters/CHAR_Pavo_Base.CHAR_Pavo_Base'"));
	//Mesh->SetStaticMesh(CharacterMeshAsset.Object);

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SkeletalMeshAsset(TEXT("SkeletalMesh'/Game/Characters/Animations/CHAR_Pavo_Walk.CHAR_Pavo_Walk'"));
	skeletalMesh->SetSkeletalMesh(SkeletalMeshAsset.Object);
	skeletalMesh->SetEnableGravity(true);
	skeletalMesh->SetSimulatePhysics(false);

	static ConstructorHelpers::FObjectFinder<UClass>AnimationBP(TEXT("AnimBlueprint'/Game/Characters/Animations/Pavo_AnimBP.Pavo_AnimBP_C'"));
	skeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	skeletalMesh->AnimClass = AnimationBP.Object;
	//skeletalMesh->SetAnimInstanceClass(AnimationBP.Object->GetAnimBlueprintGeneratedClass());

	cameraArm = CreateDefaultSubobject<USpringArmComponent>("CameraSpringArm");
	cameraArm->SetupAttachment(skeletalMesh);
	cameraArm->SetAbsolute(false, true, false);
	cameraArm->SetWorldRotation(FRotator(320.f, 270.f, 0.f));
	cameraArm->TargetArmLength = 1150.f;
	cameraArm->bDoCollisionTest = false;

	camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	camera->AttachToComponent(cameraArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);

	Speed = 0.f;

	RootComponent = skeletalMesh;//Mesh;
	
	//Create and attach the rifle and grenade
	rifle = CreateDefaultSubobject<URifle>("Rifle");
	rifle->mesh->SetVisibility(false);
	rifle->mesh->SetSimulatePhysics(false);
	rifle->mesh->SetupAttachment(skeletalMesh, FName("GunSocket"));
	rifle->mesh->SetRelativeLocation(FVector(0, 0, 0));
	rifle->mesh->SetWorldRotation(FRotator(0, 270, 90));
	
	launcher = CreateDefaultSubobject<ULauncher>("Launcher");
	launcher->mesh->SetSimulatePhysics(false);
	launcher->mesh->SetVisibility(false);
	launcher->mesh->SetupAttachment(skeletalMesh, FName("GrenadeSocket"));
	launcher->mesh->SetRelativeLocation(FVector(0, 0, 0));

	//Set to blue team's (color 02) material 
	static ConstructorHelpers::FObjectFinder<UMaterial>RedTeamMaterial(TEXT("Material'/Game/Characters/lambert1.lambert1'"));
	RedTeamColor = (UMaterial*)RedTeamMaterial.Object;
	skeletalMesh->SetMaterial(0, RedTeamColor);

	static ConstructorHelpers::FObjectFinder<UMaterial>BlueTeamMaterial(TEXT("Material'/Game/Characters/lambert1_2.lambert1_2'"));
	BlueTeamColor = (UMaterial*)BlueTeamMaterial.Object;

	//physics 
	TInlineComponentArray<UPrimitiveComponent*> Components;
	GetComponents(Components);
	
	for (UPrimitiveComponent* Component : Components) {
		Component->SetSimulatePhysics(false);
	}
	skeletalMesh->SetSimulatePhysics(false);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>ThrowAnimMontage(TEXT("AnimMontage'/Game/Characters/Animations/BlendSpaces/ThrowGrenadeMontage.ThrowGrenadeMontage'"));
	throwMontage = ThrowAnimMontage.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>ShootRifleAnimMontage(TEXT("AnimMontage'/Game/Characters/Animations/BlendSpaces/ShootRifleMontage.ShootRifleMontage'"));
	shootRifleMontage = ShootRifleAnimMontage.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>TurnLeftAnimMontage(TEXT("AnimMontage'/Game/Characters/Animations/BlendSpaces/LeftTurnMontage.LeftTurnMontage'"));
	turnLeftMontage = TurnLeftAnimMontage.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>TurnRightAnimMontage(TEXT("AnimMontage'/Game/Characters/Animations/BlendSpaces/RightTurnMontage.RightTurnMontage'"));
	turnRightMontage = TurnRightAnimMontage.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>TurnAroundAnimMontage(TEXT("AnimMontage'/Game/Characters/Animations/BlendSpaces/TurnAroundMontage1.TurnAroundMontage1'"));
	turnAroundMontage = TurnAroundAnimMontage.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>StumbleAnimMontage(TEXT("AnimMontage'/Game/Characters/Animations/BlendSpaces/StumbleMontage.StumbleMontage'"));
	stumbleMontage = StumbleAnimMontage.Object;

	facingDirection = Direction::Right;
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
		skeletalMesh->SetMaterial(0, BlueTeamColor);
		facingDirection = Direction::Left;
	 }
	 
	 
}

// Called when the game starts or when spawned
void ACrewMember::BeginPlay() {
	Super::BeginPlay();

	ALaetusGameMode* gameMode = Cast<ALaetusGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode) {
		grid = gameMode->getGameGrid();
	}
	
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
 * Rotate this ACrewMember to the target direction and begin moving them forward 
 * until they have reached the target AGridSpace.
 * 
 * @param target a pointer to the AGridSpace to move this
 *     ACrewMember to.
 */
void ACrewMember::MoveTo(AGridSpace * target) {
	controller->disable();
	targetLocation = target;
	FVector2D unitDirection = grid->getUnitDifference(gridSpace, target);
	directionToFaceEnum = vectorToDirectionEnum(unitDirection);
	float montageLength = rotateWithAnimation(directionToFaceEnum);
	
	if (montageLength > 0) {
		FTimerHandle timerParams;
		GetWorld()->GetTimerManager().SetTimer(timerParams, this, &ACrewMember::moveForward, montageLength - 0.2f, false);
	}else {
		moveForward();
	}
}

void ACrewMember::moveForward() {
	rotateToDirection(directionToFaceEnum);
	if (targetLocation == nullptr || targetLocation->isOccupied()) {
		return;
	}

	Speed = 0;

	//Calculate how much to increment movement by in each iteration of the timer.
	newLocation = targetLocation->GetActorLocation() + FVector(0, 0, 20);
	FVector oldLocation = gridSpace->GetActorLocation() + FVector(0, 0, 20);
	moveIncrement = (newLocation - oldLocation) / 150;

	// Reset pointers/references
	setGridSpace(targetLocation);

	//Start the timer to increment the position up until we reach the destination
	GetWorld()->GetTimerManager().SetTimer(moveTimerHandle, this, &ACrewMember::incrementMoveForward, 0.01, true);
}

void ACrewMember::incrementMoveForward() {
	if (Speed < 500) {
		Speed += 50;
	}

	FVector currentLocation = GetActorLocation();
	float distance = FVector::Dist(currentLocation, newLocation);
	if (FMath::Abs(distance) > 5) {
		SetActorLocation(currentLocation + moveIncrement);
	}
	else {
		Speed = 0;
		SetActorLocation(newLocation);
		GetWorld()->GetTimerManager().ClearTimer(moveTimerHandle);
		controller->enable();
	}
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
	playStumbleMontage();		
}

/**
 * Set the AGridSpace this ACrewMember is currently standing on to the given
 * AGridSpace.
 * 
 * @param space a pointer to the new AGridSpace this ACrewMember is now
 *     standing on.
 */
void ACrewMember::setGridSpace(class AGridSpace* space) {
	
	if (space && !space->isOccupied()) {

		if (gridSpace) {
			gridSpace->setOccupant(nullptr);
		}

		space->setOccupant(this);
		gridSpace = space;
	}
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

/**
 * Play the grenade throwing montage.
 */
void ACrewMember::playThrowMontage() {
	skeletalMesh->GetAnimInstance()->Montage_Play(throwMontage);
}

/**
 * Play the shooting rifle montage.
 */
void ACrewMember::playShootRifleMontage() {
	skeletalMesh->GetAnimInstance()->Montage_Play(shootRifleMontage);
}

/**
 * Rotate this ACrewMember to the given direction, and play the appropriate 
 * animation while doing so.
 * 
 * @param directionToFace a Direction enum indicating which of the four
 *     direction this ACrewMember should now face.
 * @return 0 on success, -1 otherwise
*/
float ACrewMember::rotateWithAnimation(Direction directionToFace) {
	if (facingDirection == Direction::Left) {
		if (directionToFace == Direction::Right)
			return playRotationMontage(RotationAnim::TurnAround);
		if (directionToFace == Direction::Up)
			return playRotationMontage(RotationAnim::TurnRight);
		if (directionToFace == Direction::Down)
			return playRotationMontage(RotationAnim::TurnLeft);
	}else if (facingDirection == Direction::Right) {
		if (directionToFace == Direction::Left)
			return playRotationMontage(RotationAnim::TurnAround);
		if (directionToFace == Direction::Up)
			return playRotationMontage(RotationAnim::TurnLeft);
		if (directionToFace == Direction::Down)
			return playRotationMontage(RotationAnim::TurnRight);
	}else if (facingDirection == Direction::Up) {
		if (directionToFace == Direction::Left)
			return playRotationMontage(RotationAnim::TurnLeft);
		if (directionToFace == Direction::Right)
			return playRotationMontage(RotationAnim::TurnRight);
		if (directionToFace == Direction::Down)
			return playRotationMontage(RotationAnim::TurnAround);
	}else {
		if (directionToFace == Direction::Left)
			return playRotationMontage(RotationAnim::TurnRight);
		if (directionToFace == Direction::Right)
			return playRotationMontage(RotationAnim::TurnLeft);
		if (directionToFace == Direction::Up)
			return playRotationMontage(RotationAnim::TurnAround);
	}
	return -1;
}

/**
 * Play the given rotation animation.
 * 
 * @param type the type of rotation animation to play
 * @return 0 on success, -1 otherwise
 */
float ACrewMember::playRotationMontage(RotationAnim type) {
	FOnMontageEnded b;
	b.BindUObject(this, &ACrewMember::onRotationAnimationEnd);

	float montageLength;
	switch (type) {
	case TurnLeft:
		UE_LOG(LogTemp, Warning, TEXT("Playing turn left"));
		montageLength = skeletalMesh->GetAnimInstance()->Montage_Play(turnLeftMontage);
		skeletalMesh->GetAnimInstance()->Montage_SetEndDelegate(b, turnLeftMontage);
		return montageLength;
	case TurnRight:
		UE_LOG(LogTemp, Warning, TEXT("Playing turn right"));
		montageLength = skeletalMesh->GetAnimInstance()->Montage_Play(turnRightMontage);
		skeletalMesh->GetAnimInstance()->Montage_SetEndDelegate(b, turnRightMontage);
		return montageLength;	
	case TurnAround:
		UE_LOG(LogTemp, Warning, TEXT("Playing turn around"));
		montageLength = skeletalMesh->GetAnimInstance()->Montage_Play(turnAroundMontage);
		skeletalMesh->GetAnimInstance()->Montage_SetEndDelegate(b, turnAroundMontage);
		return montageLength;
	default:
		return -1;
	}
}

/**
 * Convert the given unit direction vector to the corresponding 
 * Direction enum value. 
 * 
 * @param direction the unit direction vector to convert. Must be one of:
 *     (-1, 0) [Up], (1, 0) [Down], (0, 1) [Right], or (-1, 0) [Left]
 * @return a valid Direction enum is a valid direction FVector2D was given,
 *     Direction::InvalidDirection otherwise.
 */
Direction ACrewMember::vectorToDirectionEnum(FVector2D direction) {
	if (direction == FVector2D(-1, 0)) {
		return Direction::Up;
	}else if (direction == FVector2D(1, 0)) {
		return Direction::Down;
	}else if (direction == FVector2D(0, 1)) {
		return Direction::Right;
	}else if (direction == FVector2D(0, -1)) {
		return Direction::Left;
	}else {
		return Direction::InvalidDirection;
	}
}

float ACrewMember::getSpeed() {
	return Speed;
}

void ACrewMember::onRotationAnimationEnd(UAnimMontage* montage, bool wasInteruppted) {
	UE_LOG(LogTemp, Warning, TEXT("In callback from %s"), *montage->GetName());
}

void ACrewMember::rotateToDirection(Direction direction) {
	facingDirection = direction;
	switch (direction) {
	case Up:
		rotateUp();
		break;
	case Left:
		rotateLeft();
		break;
	case Right:
		rotateRight();
		break;
	case Down:
		rotateDown();
		break;
	}
}

void ACrewMember::rotateUp() {
	skeletalMesh->SetWorldRotation(upRotation);
}

void ACrewMember::rotateLeft() {
	skeletalMesh->SetWorldRotation(leftRotation);
}

void ACrewMember::rotateRight() {
	skeletalMesh->SetWorldRotation(rightRotation);
}

void ACrewMember::rotateDown() {
	skeletalMesh->SetWorldRotation(downRotation);
}

int ACrewMember::getTeam() {
	return team;
}

int ACrewMember::playStumbleMontage() {
	return skeletalMesh->GetAnimInstance()->Montage_Play(stumbleMontage);
}

void ACrewMember::setController(ACrewController* newController) {
	controller = newController;
}