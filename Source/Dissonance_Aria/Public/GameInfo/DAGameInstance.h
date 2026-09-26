

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
	UInventoryComponent* inventory;

public:
	UDAGameInstance();

	UFUNCTION(BlueprintCallable, Category="Inventory")
	UInventoryComponent* GetInventory() { return inventory; }
};
