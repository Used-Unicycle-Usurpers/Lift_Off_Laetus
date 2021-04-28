#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameEnums.generated.h"

UENUM()
enum Direction {
	InvalidDirection = -1,
	Left = 180,
	Right = 0,
	Up = 90,
	Down = 270
};

UENUM()
enum RotationAnim {
	TurnLeft,
	TurnRight,
	TurnAround
};


//Enum detailing the diffirent possible types of harvest sources, plus
//one for invalid type.
UENUM()
enum HarvestSourceType {
	Rock,
	SlimeTree,
	Shrub,
	Invalid
};