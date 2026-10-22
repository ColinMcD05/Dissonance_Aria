

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class E_CombatDirections : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Up = 1 UMETA(DisplayName = "Up"),
	Down = 2 UMETA(DisplayName = "Down"),
	Right = 3 UMETA(DisplayName = "Right"),
	Left = 4 UMETA(DisplayName = "Left")
};

UENUM(BlueprintType)
enum class E_Directions : uint8
{
	None UMETA(DisplayName = "None"),
	Forward UMETA(DisplayName = "Forward"),
	Backward UMETA(DisplayName = "Backward"),
	Right UMETA(DisplayName = "Right"),
	Left UMETA(DisplayName = "Left")
};