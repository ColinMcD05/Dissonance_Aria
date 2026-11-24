


#include "GameInfo/DAGameInstance.h"

UDAGameInstance::UDAGameInstance()
{
	inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
}

void UDAGameInstance::SetInventory(UInventoryComponent* newInventory)
{
	inventory->SetWeapon(newInventory->GetWeapon());
}

void UDAGameInstance::ResetInfo()
{
	lastCamera = nullptr;
	if (IsValid(inventory))
		inventory->ResetInventory();
}