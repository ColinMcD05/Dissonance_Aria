

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
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "DamageableInterface")
	float GetCurrentHealth();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "DamageableInterface")
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "DamageableInterface")
	bool GetIsDead();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "DamageableInterface")
	void Heal(float HealAmount, AActor* Healer);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "DamageableInterface")
	bool TakeDamage(FS_DamageInfo& DamageInfo);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "DamageableInterface")
	void HandleDeath(AActor* killer);
};
