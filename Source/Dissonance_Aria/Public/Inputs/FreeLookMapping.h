

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Inputs/FreeLook.h"
#include "FreeLookMapping.generated.h"

/**
 * 
 */
UCLASS()
class DISSONANCE_ARIA_API UFreeLookMapping : public UInputMappingContext
{
	GENERATED_BODY()
	
public:
	UFreeLookMapping();

	UFreeLook* freeLook;
};
