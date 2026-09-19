

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageSystem/DamageInfo.h"
#include "DamageableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DISSONANCE_ARIA_API IDamageableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DamageableInterface")
	float GetCurrentHealth();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DamageableInterface")
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DamageableInterface")
	bool GetIsDead();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DamageableInterface")
	void Heal(float HealAmount, AActor* Healer);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DamageableInterface")
	bool TakeDamage(FDamageInfo DamageInfo);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "DamageableInterface")
	bool HandleDeath();
};
