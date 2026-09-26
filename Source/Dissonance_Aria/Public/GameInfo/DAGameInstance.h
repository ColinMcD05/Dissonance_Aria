

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Inventory/InventoryComponent.h"
#include "DAGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DISSONANCE_ARIA_API UDAGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetInventory, BlueprintSetter = SetInventory, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* inventory;

public:
	UDAGameInstance();

	UFUNCTION(BlueprintCallable, BlueprintGetter, Category="Inventory")
	UInventoryComponent* GetInventory() { return inventory; }

	UFUNCTION(BlueprintCallable, BlueprintSetter, Category = "Inventory")
	void SetInventory(UInventoryComponent* newInventory);
};
