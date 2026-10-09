

#pragma once

#include "ControllerTypeInfo.generated.h"

UENUM(BlueprintType)
enum class E_ControllerType : uint8
{
	Other UMETA(DisplayName = "None"),
	Keyboard UMETA(DisplayName = "Keyboard"),
	Controller UMETA(DisplayName = "Controller")
};