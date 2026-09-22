

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/WeaponInfo.h"
#include "InventoryComponent.generated.h"

#define MAX_WEAPONS_AMOUNT 4

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISSONANCE_ARIA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

#pragma region Weapons
	TArray<FS_WeaponInfo> weapons;

	int weaponOneIndex = 0;
	int weaponTwoIndex;
#pragma endregion


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddNewWeapon(const FS_WeaponInfo newWeapon);
	//Getters
	//Returns weapon struct based on the index
	FS_WeaponInfo* GetWeaponAtIndex(int index);

	//Returns weapon struct based on the type
	FS_WeaponInfo* GetWeaponByType(E_WeaponType weaponType);

	//Returns the whole weapon array
	UFUNCTION(BlueprintCallable, Category = "Getter")
	TArray<FS_WeaponInfo>& GetWeaponsArray() { return weapons; }

	//Swaps what weapons are equipped
	UFUNCTION(BlueprintCallable, Category = "Swap")
	bool SwapEquipedWeapons(E_WeaponType weaponToEquip, E_WeaponType weaponToUnequip);

	//Checks if the weapon is currently equipped
	bool CheckIfWeaponEquipped(E_WeaponType weaponType);

	//Checks if the weapon if currently in the inventory
	bool CheckIfWeaponInInventory(E_WeaponType weaponType);
	
	/*Finds whether weapon is equipped weapon 1 or 2. returns - 1 for failure*/
	int WhichEquippedWeapon(E_WeaponType weaponType);

	//Returns the index of the weapon base on type -1 for failure
	int GetIndexByWeaponType(E_WeaponType weaponType);
};
