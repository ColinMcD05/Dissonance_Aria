


#include "Inputs/FreeLookMapping.h"
#include "InputModifiers.h"

UFreeLookMapping::UFreeLookMapping()
{
	freeLook = CreateDefaultSubobject<UFreeLook>(TEXT("FreeLook"));

	FEnhancedActionKeyMapping& firstMapping = MapKey(freeLook, EKeys::Mouse2D);
	firstMapping.Modifiers.Add(CreateDefaultSubobject<UInputModifierNegate>("Negate"));

	FEnhancedActionKeyMapping&  secondMapping = MapKey(freeLook, EKeys::Gamepad_Right2D);
	secondMapping.Modifiers.Add(CreateDefaultSubobject<UInputModifierScalar>("Scalar"));
}