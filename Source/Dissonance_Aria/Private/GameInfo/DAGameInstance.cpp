


#include "GameInfo/DAGameInstance.h"

UDAGameInstance::UDAGameInstance()
{
	inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
}

void UDAGameInstance::SetInventory(UInventoryComponent* newInventory)
{
	for (int i = 0; i < inventory->GetWeaponsLength(); i++)
	{
		inventory->AddNewWeapon(*newInventory->GetWeaponAtIndex(i));
	}

	inventory->SetWeaponOne(newInventory->GetEquippedWeaponOneIndex());
	inventory->SetWeaponTwo(newInventory->GetEquippedWeaponTwoIndex());
}