

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
	UPROPERTY(EditAnywhere, BlueprintGetter = GetInventory, BlueprintSetter = SetInventory, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* inventory;

	FString lastCamera;
public:
	UDAGameInstance();

	UFUNCTION(BlueprintCallable, BlueprintGetter, Category="Inventory")
	UInventoryComponent* GetInventory() { return inventory; }

	UFUNCTION(BlueprintCallable, BlueprintSetter, Category = "Inventory")
	void SetInventory(UInventoryComponent* newInventory);

	UFUNCTION(BlueprintCallable)
	FString GetLastCamera() {return lastCamera;}

	UFUNCTION(BlueprintCallable)
	void SetLastCamera(FString newCamera) { lastCamera = newCamera; }
};
