


#include "GameInfo/DAGameInstance.h"

UDAGameInstance::UDAGameInstance()
{
	inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
}