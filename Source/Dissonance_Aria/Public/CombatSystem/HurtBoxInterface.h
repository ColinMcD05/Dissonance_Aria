

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HurtBoxInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHurtBoxInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DISSONANCE_ARIA_API IHurtBoxInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Hit")
	bool OverlappedActor(AActor* hitEnemy);
};
