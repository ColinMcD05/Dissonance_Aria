


#include "Weapons/WeaponsSystemComponent.h"
#include "Player/PlayerCharacterCombat.h"

// Sets default values for this component's properties
UWeaponsSystemComponent::UWeaponsSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = false;
}


// Called when the game starts
void UWeaponsSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UWeaponsSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UWeaponsSystemComponent::SwapWeapons()
{
	if (canSwap)
	{
		AWeaponActor* tempWeapon = currentHeldWeapon;

		currentHeldWeapon = storedWeapon;
		currentHeldWeapon->Activate();

		storedWeapon = tempWeapon;
		storedWeapon->Deactivate();
		canSwap = false;

		GetWorld()->GetTimerManager().SetTimer(swapWeaponsTimer, this, &UWeaponsSystemComponent::ResetCanSwap, 2, false);

		return true;
	}

	return false;
}

void UWeaponsSystemComponent::SpawnWeapons(UInventoryComponent* inventory, APlayerCharacterCombat* player)
{
	FS_WeaponInfo* weaponOne = inventory->GetWeaponOne();
	if (weaponOne)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = player;
		spawnParams.Instigator = player->GetInstigator();

		currentHeldWeapon = GetWorld()->SpawnActor<AWeaponActor>(weaponOne->weaponActor, player->GetActorTransform(), spawnParams);
		currentHeldWeapon->InitializeWeapon(player, *weaponOne);
		currentHeldWeapon->Activate();
	}

	FS_WeaponInfo* weaponTwo = inventory->GetWeaponTwo();
	if (weaponTwo)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = player;
		spawnParams.Instigator = player->GetInstigator();

		storedWeapon = GetWorld()->SpawnActor<AWeaponActor>(weaponTwo->weaponActor, player->GetActorTransform(), spawnParams);
		storedWeapon->InitializeWeapon(player, *weaponTwo);
		storedWeapon->Deactivate();
	}
}