

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class E_CombatDirections : uint8
{
	None = 99 UMETA(DisplayName = "None"),
	Up = 0 UMETA(DisplayName = "Up"),
	Down = 1 UMETA(DisplayName = "Down"),
	Right = 2 UMETA(DisplayName = "Right"),
	Left = 3 UMETA(DisplayName = "Left")
};