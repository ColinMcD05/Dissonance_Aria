


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
		currentHeldWeapon->AttachToComponent(player->GetWeaponSpot(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
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
		storedWeapon->AttachToComponent(player->GetWeaponSpot(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		storedWeapon->Deactivate();
	}
}


bool UWeaponsSystemComponent::SwapWeapons(AWeaponActor*& swapTo, int whichWeapon)
{
	if (canSwapWeapon)
	{
		if (swapTo != currentHeldWeapon)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Yes"));
			}
			AWeaponActor* tempWeapon = currentHeldWeapon;

			currentHeldWeapon = storedWeapon;
			currentHeldWeapon->Activate();

			storedWeapon = tempWeapon;
			storedWeapon->Deactivate();
			canSwapWeapon = false;

			GetWorld()->GetTimerManager().SetTimer(swapWeaponsTimer, this, &UWeaponsSystemComponent::ResetCanSwapWeapon, 2, false);

			OnWeaponSwapped.Broadcast(whichWeapon);
			OnChangeMusic.Broadcast(static_cast<int32>(currentHeldWeapon->GetWeaponInfo().genre));

			return true;
		}
		return false;
	}

	return false;
}

void UWeaponsSystemComponent::SwapTunings(int tuning)
{
	if (canSwapTuning && currentHeldWeapon)
	{
		int swap = currentHeldWeapon->SwapTuning(tuning);
		if (swap != -1)
		{
			canSwapTuning = false;

			OnTuningSwapped.Broadcast(tuning);
			OnChangeMusic.Broadcast(static_cast<int32>(currentHeldWeapon->GetWeaponInfo().genre) + currentHeldWeapon->GetCurrentTuning());

			GetWorld()->GetTimerManager().SetTimer(swapWeaponsTimer, this, &UWeaponsSystemComponent::ResetCanSwapWeapon, 2, false);
		}
	}
}

void  UWeaponsSystemComponent::ResetCanSwapWeapon()
{
	canSwapWeapon = true;
}

void  UWeaponsSystemComponent::ResetCanSwapTuning()
{
	canSwapTuning = true;
}