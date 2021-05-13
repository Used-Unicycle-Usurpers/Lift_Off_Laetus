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
#include "../Controllers/InputController.h"
#include "CharacterAnimDataAsset.h"
#include "Sound/SoundCue.h"

//Data assets with all mesh, material, and animation information
//for each of the three characters.
UCharacterAnimDataAsset* pavoData;
UCharacterAnimDataAsset* lyraData;
UCharacterAnimDataAsset* nembusData;

#define RED_TEAM_MATERIAL "Material'/Game/Characters/Pavo1.Pavo1'"
#define BLUE_TEAM_MATERIAL "Material'/Game/Characters/Pavo2.Pavo2'"

// Sets default values
ACrewMember::ACrewMember() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Get the data assets for all three charcters. Will be loaded up later
	//when we know which character this is.
	static ConstructorHelpers::FObjectFinder<UCharacterAnimDataAsset>PavoData(TEXT("CharacterAnimDataAsset'/Game/Characters/PavoAnimDataAsset.PavoAnimDataAsset'"));
	pavoData = PavoData.Object;
	static ConstructorHelpers::FObjectFinder<UCharacterAnimDataAsset>LyraData(TEXT("CharacterAnimDataAsset'/Game/Characters/LyraAnimDataAsset.LyraAnimDataAsset'"));
	lyraData = LyraData.Object;
	static ConstructorHelpers::FObjectFinder<UCharacterAnimDataAsset>NembusData(TEXT("CharacterAnimDataAsset'/Game/Characters/NembusAnimDataAsset.NembusAnimDataAsset'"));
	nembusData = NembusData.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue>sound(TEXT("SoundCue'/Game/Audio/Weapons/AUD_punch_Cue.AUD_punch_Cue'"));
	punchSound = sound.Object;
	
	//Intialize the skeletal mesh component. The mesh itself will be specified
	//in setMeshAnimData.
	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	skeletalMesh->SetEnableGravity(true);
	skeletalMesh->SetSimulatePhysics(false);
	skeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	RootComponent = skeletalMesh;

	//Add a camera with a spring arm so we can move the camera to this ACrewMember.
	cameraArm = CreateDefaultSubobject<USpringArmComponent>("CameraSpringArm");
	cameraArm->SetupAttachment(skeletalMesh);
	cameraArm->SetAbsolute(false, true, false);
	cameraArm->SetWorldRotation(FRotator(320.f, 270.f, 0.f));
	cameraArm->TargetArmLength = 1150.f;
	cameraArm->bDoCollisionTest = false;

	camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	camera->AttachToComponent(cameraArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);

	Speed = 0.f;

	//Create and attach the rifle and grenade
	rifle = CreateDefaultSubobject<URifle>("Rifle");
	rifle->mesh->SetVisibility(false);
	rifle->mesh->SetSimulatePhysics(false);
	
	launcher = CreateDefaultSubobject<ULauncher>("Launcher");
	launcher->mesh->SetSimulatePhysics(false);
	launcher->mesh->SetVisibility(false);

	//physics 
	TInlineComponentArray<UPrimitiveComponent*> Components;
	GetComponents(Components);
	
	for (UPrimitiveComponent* Component : Components) {
		Component->SetSimulatePhysics(false);
	}
	skeletalMesh->SetSimulatePhysics(false);

	facingDirection = Direction::Right;
}


// Called when the game starts or when spawned
void ACrewMember::BeginPlay() {
	Super::BeginPlay();

	gameMode = Cast<ALaetusGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode) {
		grid = gameMode->getGameGrid();
	}

	health = maxHealth;
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
 * Loads up the CharacterAnimDataAsset and assigns the corresponding
 * meshes, materials, and animations beased on the provided character
 * and team.
 * 
 * @param character the character this ACrewMember will be. The mesh,
 *     materials, and animations for this character will be loaded and 
 *     assigned to this ACrewMember.
 */
void ACrewMember::setMeshAnimData(FCharacter character, Team playerTeam) {
	//Pick the corresonding CharacterAnimDataAsset.
	UCharacterAnimDataAsset* data;
	switch (character) {
	case Pavo:
		data = pavoData;
		break;
	case Lyra:
		data = lyraData;
		break;
	default:
		data = nembusData;
		break;
	}

	//Get all the animation montages.
	throwMontage = data->throwMontage;
	throwMontageDelay = data->throwMontageDelay;
	shootRifleMontage = data->shootRifleMontage;
	punchMontage = data->punchMontage;
	takeDamageMontage = data->takeDamageMontage;
	deathMontage = data->deathMontage;
	turnLeftMontage = data->turnLeftMontage;
	turnRightMontage = data->turnRightMontage;
	turnAroundMontage = data->turnAroundMontage;
	stumbleMontage = data->stumbleMontage;
	pushMontage = data->pushMontage;

	//Get all sound effects for this particular character.
	deathSound = data->deathSound;
	takeDamageSound = data->takeDamageSound;
	slipSound = data->slipSound;
	footstepSound = data->footstepSound;

	//Get the skeletal mesh, but don't assign the materials until we also 
	//have the rifle mesh ready.
	skeletalMesh->SetSkeletalMesh(data->skeletalMesh);
	skeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	skeletalMesh->SetAnimClass(data->animBP);

	//Create and attach the rifle and grenade to their respective sockets on
	//the skeletal mesh.
	FAttachmentTransformRules params = FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false);

	rifle->mesh->AttachToComponent(skeletalMesh, params, FName("GunSocket"));
	rifle->mesh->SetRelativeLocation(FVector(0, 0, 0));
	rifle->mesh->SetRelativeRotation(FRotator(0, 0, 0));

	launcher->mesh->AttachToComponent(skeletalMesh, params, FName("GrenadeSocket"));
	launcher->mesh->SetRelativeLocation(FVector(0, 0, 0));

	//Based on the provided team, assigned the corresponding materials to the
	//skeletal mesh and rifle.
	switch (character) {
	case Pavo:
		if (playerTeam == Team::Red) {
			skeletalMesh->SetMaterial(0, data->redTeamMainMaterial);
			rifle->mesh->SetMaterial(0, data->redGunSightMaterial);
			rifle->mesh->SetMaterial(1, data->redGunBodyMaterial);
		}else {
			skeletalMesh->SetMaterial(0, data->blueTeamMainMaterial);
			rifle->mesh->SetMaterial(0, data->blueGunSightMaterial);
			rifle->mesh->SetMaterial(1, data->blueGunBodyMaterial);
		}
		break;
	case Lyra:
		if (playerTeam == Team::Red) {
			skeletalMesh->SetMaterial(0, data->redTeamMainMaterial);
			skeletalMesh->SetMaterial(1, data->redTeamArmorMaterial);
			rifle->mesh->SetMaterial(0, data->redGunSightMaterial);
			rifle->mesh->SetMaterial(1, data->redGunBodyMaterial);
		}else {
			skeletalMesh->SetMaterial(0, data->blueTeamMainMaterial);
			skeletalMesh->SetMaterial(1, data->blueTeamArmorMaterial);
			rifle->mesh->SetMaterial(0, data->blueGunSightMaterial);
			rifle->mesh->SetMaterial(1, data->blueGunBodyMaterial);
		}
		break;
	default:
		if (playerTeam == Team::Red) {
			skeletalMesh->SetMaterial(0, data->redTeamMainMaterial);
			skeletalMesh->SetMaterial(1, data->redTeamArmorMaterial);
			rifle->mesh->SetMaterial(0, data->redGunSightMaterial);
			rifle->mesh->SetMaterial(1, data->redGunBodyMaterial);
		}else {
			skeletalMesh->SetMaterial(0, data->blueTeamMainMaterial);
			skeletalMesh->SetMaterial(1, data->blueTeamArmorMaterial);
			rifle->mesh->SetMaterial(0, data->blueGunSightMaterial);
			rifle->mesh->SetMaterial(1, data->blueGunBodyMaterial);
		}
		break;
	}
}

/**
 * Set this ACrewMember's team to the given team
 * 
 * @param newTeam the new team to assign to this ACrewMember.
 */
 void ACrewMember::SetTeam(Team newTeam) {
	 if (newTeam) {
		 team = newTeam;
	 } 

	 //Change appearance based on team
	 //Red  - Color 01 - default
	 //Blue - Color 02 - update material 
	 if (team == Team::Blue) {
		facingDirection = Direction::Left;
	 }else {
		 facingDirection = Direction::Right;
	 }
}

 /**
  * Return the team this ACrewMember is a part of.
  *
  * @return 0 if this ACrewMember is on the red team,
  *     1 if on the blue team.
  */
 Team ACrewMember::getTeam() {
	 return team;
 }

/**
 * Rotate this ACrewMember to the target direction and begin moving them forward 
 * until they have reached the target AGridSpace.
 * 
 * @param target a pointer to the AGridSpace to move this
 *     ACrewMember to.
 */
void ACrewMember::MoveTo(AGridSpace * target, bool pushingCoreFragment) {
	controller->disableInputController();
	targetLocation = target;
	pushing = pushingCoreFragment;
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

/**
 * Play the walking animation and inch the ACrewMember forward to 
 * the AGridSpace they're moving to.
 */
void ACrewMember::moveForward() {
	rotateToDirection(directionToFaceEnum);
	if (targetLocation == nullptr || targetLocation->isOccupied()) {
		return;
	}

	if (pushing) {
		playPushMontage();
	}else {
		Speed = 0;
	}

	//Calculate how much to increment movement by in each iteration of the timer.
	newLocation = targetLocation->GetActorLocation() + FVector(0, 0, 20);
	FVector oldLocation = gridSpace->GetActorLocation() + FVector(0, 0, 20);
	incrementsLeft = numIncrements = 75;
	moveIncrement = (newLocation - oldLocation) / numIncrements;

	// Reset pointers/references
	setGridSpace(targetLocation);

	//Start the timer to increment the position up until we reach the destination
	UGameplayStatics::PlaySound2D(GetWorld(), footstepSound);
	GetWorld()->GetTimerManager().SetTimer(moveTimerHandle, this, &ACrewMember::incrementMoveForward, 0.01, true);
}

/**
 * Called by the looping timer in moveForward. A single call to this 
 * function moves the ACrewMember forward by the amount in moveIncrement.
 */
void ACrewMember::incrementMoveForward() {
	if (Speed < 500) {
		Speed += 50;
	}

	FVector currentLocation = GetActorLocation();
	float distance = FVector::Dist(currentLocation, newLocation);

	//If in a certain distance tolerance of the actual location, consider 
	//the movement completed. This handles cases where moveIncrement does 
	//not add up to exactly the destination location.
	incrementsLeft--;
	if (incrementsLeft > 0) {
		//Destination has not been reached, increment position
		SetActorLocation(currentLocation + moveIncrement);
	}else {
		//Desination has been reached! Stop timer.
		Speed = 0;
		SetActorLocation(newLocation);//Snap to the exact location
		GetWorld()->GetTimerManager().ClearTimer(moveTimerHandle);
		controller->enableInputController();
		//change turn if actionBar is 0
		if (gameMode->getABStatus() == 0) { gameMode->ChangeTurn(); }
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
void ACrewMember::Shoot(FVector2D target, bool useRifle) {
	if (useRifle) {
		rifle->fire(target);
	} else {
		launcher->fire(target);
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
 * Rotate the player in the given direction and then punch at the ACrewMember 
 * in the adjacent AGridSpace in that given direction, if there is indeed an 
 * ACrewMember ther.
 * 
 * @param direction the cardinal direction to punch towards.
 */
void ACrewMember::Punch(FVector2D direction) {
	controller->disableInputController();
	targetLocation = grid->getTile(gridSpace->getGridLocation() + direction);
	directionToFaceEnum = vectorToDirectionEnum(direction);
	float montageLength = rotateWithAnimation(directionToFaceEnum);

	if (montageLength > 0) {
		FTimerHandle timerParams;
		GetWorld()->GetTimerManager().SetTimer(timerParams, this, &ACrewMember::punchAtDirection, montageLength, false);
	}else {
		punchAtDirection();
	}
}

/**
 * Play the punch montage, and set timers to deal damage and 
 * re-enable input.
 */
void ACrewMember::punchAtDirection() {
	rotateToDirection(directionToFaceEnum);
	
	float montageLength = playPunchMontage();
	
	//Halfway thorough the punch montage (about when the actual 
	//punch happens), deal damage.
	FTimerHandle damageTimer;
	GetWorld()->GetTimerManager().SetTimer(damageTimer, this, &ACrewMember::dealPunchDamage, montageLength/2, false);

	//Renable input after the montage had ended.
	FTimerHandle enableTimer;
	GetWorld()->GetTimerManager().SetTimer(enableTimer, this, &ACrewMember::enableInputAfterPunch, montageLength, false);
}

/**
 * Deal punch damage to the occupant of targetLocation if 
 * they are an ACrewMember.
 */
void ACrewMember::dealPunchDamage() {
	UGameplayStatics::PlaySound2D(GetWorld(), punchSound);
	ACrewMember* occupant = Cast<ACrewMember>(targetLocation->getOccupant());
	if (occupant) {
		occupant->takeDamage(1.0f);
	}
}

/**
 * Re-enables input after the punch has occurred.
 */
void ACrewMember::enableInputAfterPunch() {
	getCrewController()->enableInputController();

	//change turn if actionBar is 0
	if (gameMode->getABStatus() == 0) { gameMode->ChangeTurn(); }
}

/**
 * Reduce this ACrewMember's health by the given damage.
 * 
 * @param damageTaken the amount of damage to reduce this ACrewMember's 
 *     health by
 */
void ACrewMember::takeDamage(int32 damageTaken) {
	health -= damageTaken;
	float montageLength = playTakeDamageMontage();
	UGameplayStatics::PlaySound2D(GetWorld(), takeDamageSound);

	if (health <= 0) {
		//Play death montage. After that ends, respawn but moving to 
		//random space on your team's side of the map.
		FTimerHandle f;
		GetWorld()->GetTimerManager().SetTimer(f, this, &ACrewMember::die, montageLength, false);
	}
}

/**
 * Called when this ACrewMember has died (i.e. health <= 0). Plays 
 * the death montage and calls respawn once montage has ended.
 */
void ACrewMember::die() {
	float montageLength = playDeathMontage();
	UGameplayStatics::PlaySound2D(GetWorld(), deathSound);

	FTimerHandle timerParams;
	GetWorld()->GetTimerManager().SetTimer(timerParams, this, &ACrewMember::respawn, montageLength, false);
}

/**
 * Find a valid respawn point, and move this ACrewMember there. Also resets 
 * the corresponding AInputController's currentlySelectedTile to this new 
 * location.
 */
void ACrewMember::respawn() {
	AGridSpace* newSpace = grid->getValidRespawnSpace(this);
	SetActorLocation(newSpace->GetActorLocation() + FVector(0, 0, 20));
	setGridSpace(newSpace);
	getCrewController()->getInputController()->currentlySelectedTile = gridSpace;
	health = maxHealth;
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
float ACrewMember::playThrowMontage() {
	return skeletalMesh->GetAnimInstance()->Montage_Play(throwMontage);
}

/**
 * Play the shooting rifle montage.
 */
float ACrewMember::playShootRifleMontage() {
	return skeletalMesh->GetAnimInstance()->Montage_Play(shootRifleMontage);
}

/**
 * Play the punch montage.
 */
float ACrewMember::playPunchMontage() {
	return skeletalMesh->GetAnimInstance()->Montage_Play(punchMontage);
}

/**
 * Play the take damage montage.
 */
float ACrewMember::playTakeDamageMontage() {
	return skeletalMesh->GetAnimInstance()->Montage_Play(takeDamageMontage);
}

/**
 * Play the death montage.
 */
float ACrewMember::playDeathMontage() {
	return skeletalMesh->GetAnimInstance()->Montage_Play(deathMontage);
}

/**
 * Play the stumble montage.
 */
float ACrewMember::playStumbleMontage() {
	return skeletalMesh->GetAnimInstance()->Montage_Play(stumbleMontage);
}

/**
 * Play the push montage (used when pushing a core fragment).
 */
float ACrewMember::playPushMontage() {
	return skeletalMesh->GetAnimInstance()->Montage_Play(pushMontage);
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

/**
 * Rotates this ACrewMember in world space to given direction.
 * 
 * @param direction the new direction this ACrewMember should face. If 
 *     this is not one of the four cardinal directions, then the 
 *     ACrewMember will not be rotated.
 */
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

/**
 * Rotate this ACrewMember to face upward i.e. away from the screen.
 */
void ACrewMember::rotateUp() {
	skeletalMesh->SetWorldRotation(upRotation);
}

/**
 * Rotate this ACrewMember to face left.
 */
void ACrewMember::rotateLeft() {
	skeletalMesh->SetWorldRotation(leftRotation);
}

/**
 * Rotate this ACrewMember to face right.
 */
void ACrewMember::rotateRight() {
	skeletalMesh->SetWorldRotation(rightRotation);
}

/**
 * Rotate this ACrewMember to face downward i.e. towards the screen.
 */
void ACrewMember::rotateDown() {
	skeletalMesh->SetWorldRotation(downRotation);
}

/**
 * Callback for when an animation montage ends. Currently not being used, but
 * might be used in the near future. If not, this can be taken out.
 */
void ACrewMember::onRotationAnimationEnd(UAnimMontage* montage, bool wasInteruppted) {
	UE_LOG(LogTemp, Warning, TEXT("In callback from %s"), *montage->GetName());
}

bool ACrewMember::needToRotate(FVector2D newDirection) {
	Direction newDirectionEnum = vectorToDirectionEnum(newDirection);
	return facingDirection != newDirectionEnum;
}

/**
 * Returns the current value of the Speed variable.
 *
 * NOTE: Currently this variable is not used during movement, so it
 * is only used to tell the animation blueprint that this ACrewMember
 * is currently moving or not.
 *
 * TODO: Switch to a boolean if we never end up using this for anything else.
 */
float ACrewMember::getSpeed() {
	return Speed;
}

/**
 * Set the reference to the controller for the ACrew that this ACrewMember is a 
 * part of.
 * NOTE: this is a reference to the controller that is possessing the ACrew. No
 * controller is directly possessing any of the ACrewMembers.
 * 
 * @param newController a reference to the controller for the ACrew that this 
 *     ACrewMember is a part of.
 */
void ACrewMember::setCrewController(ACrewController* newController) {
	controller = newController;
}

/**
 * Get a refernce the controller that possess the ACrew this ACrewMember is a
 * part of.
 * 
 * @return a pointer to the ACrewController that possesses the ACrew this 
 *     ACrewMember is a part of.
 */
ACrewController* ACrewMember::getCrewController() {
	return controller;
}

/**
 * Returns the current amount of health this ACrewMember has.
 * 
 * @return a float representing the current amount of health 
 *     this ACrewMember has.
 */
float ACrewMember::getCurrentHealth() {
	return health;
}

/**
 * Returns true if this ACrewMember is currently standing in an AGridSpace
 * in which they can harvest from an adjacent AHarvestSource.
 */
bool ACrewMember::isNextToHarvestSource() {
	return IsValid(gridSpace->getHarvestSource());
}