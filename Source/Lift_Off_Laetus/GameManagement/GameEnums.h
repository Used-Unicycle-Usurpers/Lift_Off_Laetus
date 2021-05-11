#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameEnums.generated.h"

//Enum detailing the diffirent possible types of harvest sources, plus
//one for invalid type.
UENUM()
enum HarvestSourceType {
	Rock,
	SlimeTree,
	Shrub,
	Invalid
};

//Enum representing the four carnidal directions for player actions
UENUM()
enum Direction {
	InvalidDirection = -1,
	Left = 180,
	Right = 0,
	Up = 90,
	Down = 270
};

//Enum representing the different possible of rotations animation 
//an ACrewMember can do.
UENUM()
enum RotationAnim {
	TurnLeft,
	TurnRight,
	TurnAround
};

UENUM()
enum FCharacter {
	Pavo = 0,
	Lyra = 1,
	Nembus = 2
};

UENUM()
enum Team {
	Red,
	Blue
};