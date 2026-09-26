

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Input/InputActions/CombatActionBase.h"
#include "AttacksInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttacksInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DISSONANCE_ARIA_API IAttacksInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//Performs light attacks and returns animation length
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Attacks")
	float LightAttack(TArray<E_CombatActionType>& previousActions);

	//Performs heavy attacks and returns animation length
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Attacks")
	float HeavyAttack(TArray<E_CombatActionType>& previousActions, bool charged);

	//Performs special attacks and returns animation length
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Attacks")
	float SpecialAttack();
};
