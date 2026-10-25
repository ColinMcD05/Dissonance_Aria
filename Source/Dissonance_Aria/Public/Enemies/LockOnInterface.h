

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LockOnInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULockOnInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DISSONANCE_ARIA_API ILockOnInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "LockOn")
	void LockedOn();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "LockOn")
	void LockedOff();
};
