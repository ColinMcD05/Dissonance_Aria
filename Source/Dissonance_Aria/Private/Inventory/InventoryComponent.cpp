


#include "Inventory/InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	weapons.Reserve(MAX_WEAPONS_AMOUNT);
}

void UInventoryComponent::SetupInventory()
{

}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddNewWeapon(const FS_WeaponInfo newWeapon)
{
	for (int i = 0; i < MAX_WEAPONS_AMOUNT; i++)
	{
		if (weapons[i].weaponType == newWeapon.weaponType && !weapons[i].aquired)
		{
			weapons[i] = newWeapon;
			return;
		}
	}
}

FS_WeaponInfo* UInventoryComponent::GetWeaponAtIndex(int index)
{
	if (weapons.IsEmpty() || index >= MAX_WEAPONS_AMOUNT || !weapons[index].aquired)
	{
		return NULL;
	}

	return &weapons[index];
}

FS_WeaponInfo* UInventoryComponent::GetWeaponByType(E_WeaponType weaponType)
{
	for (int i = 0; i < MAX_WEAPONS_AMOUNT; i++)
	{
		if (weapons[i].weaponType == weaponType)
		{
			return &weapons[i];
		}
	}
	return NULL;
}

FS_WeaponInfo* UInventoryComponent::GetWeaponOne()
{
	return GetWeaponAtIndex(weaponOneIndex);
}

FS_WeaponInfo* UInventoryComponent::GetWeaponTwo()
{
	return GetWeaponAtIndex(weaponTwoIndex);
}

bool UInventoryComponent::SwapEquipedWeapons(E_WeaponType weaponToEquip, E_WeaponType weaponToUnequip)
{
	//Checks if both items are in the inventory
	if (!CheckIfWeaponInInventory(weaponToEquip) || !CheckIfWeaponInInventory(weaponToUnequip))
	{
		return false;
	}

	//Checks if the weapon to unequip is currently equipped
	if(CheckIfWeaponEquipped(weaponToUnequip))
	{
		return false;
	}

	//Checks if the weapon to equip is currently equipped
	if (CheckIfWeaponEquipped(weaponToEquip))
	{
		//Swaps weapon indexes
		int tempIndex = weaponOneIndex;
		weaponOneIndex = weaponTwoIndex;
		weaponTwoIndex = tempIndex;
		return true;
	}

	if (WhichEquippedWeapon(weaponToUnequip) == 1)
	{
		weaponOneIndex = GetIndexByWeaponType(weaponToEquip);
		return true;
	}
	else if (WhichEquippedWeapon(weaponToUnequip) == 2)
	{
		weaponTwoIndex = GetIndexByWeaponType(weaponToEquip);
		return true;
	}
	return false;
}

bool UInventoryComponent::CheckIfWeaponEquipped(E_WeaponType weaponType)
{
	if (weapons[weaponOneIndex].weaponType == weaponType)
	{
		return true;
	}
	else if (weapons[weaponTwoIndex].weaponType == weaponType)
	{
		return true;
	}
	return false;
}

bool UInventoryComponent::CheckIfWeaponInInventory(E_WeaponType weaponType)
{
	for (FS_WeaponInfo info : weapons)
	{
		if (info.weaponType == weaponType)
		{
			return info.aquired;
		}
	}
	return false;
}

int UInventoryComponent::WhichEquippedWeapon(E_WeaponType weaponType)
{
	if (weapons[weaponOneIndex].weaponType == weaponType)
	{
		return 1;
	}
	if (weapons[weaponTwoIndex].weaponType == weaponType)
	{
		return 2;
	}

	return -1;
}

int UInventoryComponent::GetIndexByWeaponType(E_WeaponType weaponType)
{
	for (int i = 0; i < MAX_WEAPONS_AMOUNT; i++)
	{
		if (weapons[i].weaponType == weaponType)
		{
			return i;
		}
	}
	return -1;
}