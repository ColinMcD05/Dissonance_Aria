


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

#pragma region Weapons
//Spawns in the weapons
void UWeaponsSystemComponent::SpawnWeapons(UInventoryComponent* inventory, APlayerCharacterCombat* thisPlayer)
{
	if (!IsValid(inventory))
		return;

	player = thisPlayer;

	//Collect the inventory weapon
	FS_WeaponInfo* weaponOne = inventory->GetWeapon();
	//Create first weapon
	if (weaponOne)
	{
		//Create the spawn parameters
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = player;
		spawnParams.Instigator = player->GetInstigator();

		//Spawn and initialize weapons
		if (weaponOne->weaponActor)
		{
			currentHeldWeapon = GetWorld()->SpawnActor<AWeaponActor>(weaponOne->weaponActor, player->GetActorTransform(), spawnParams);
			currentHeldWeapon->InitializeWeapon(player, *weaponOne);

			//Attach to component
			currentHeldWeapon->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "Weapon");
			currentHeldWeapon->Activate();
			player->UpdateWalkSpeeds(currentHeldWeapon->GetStats().speeds);
		}
	}

	//Setup Tunings
	equippedTunings = inventory->GetEquippedTunings();

	currentTuning = &baseTuning;
	OnChangeMusic.Broadcast(static_cast<int8>(currentTuning->genre));
	GiveTuningInfo();
}
#pragma endregion

#pragma region Tuning
//Swap weapon tunings
void UWeaponsSystemComponent::SwapTunings(int tuning)
{
	if (tuning == -1)
	{
		currentTuning = &baseTuning;
	}
	else
	{
		currentTuning = &equippedTunings[tuning];
	}

	OnChangeMusic.Broadcast(static_cast<int8>(currentTuning->genre));

	canSwapTuning = false;
	GetWorld()->GetTimerManager().SetTimer(swapTuningTimer, this, &UWeaponsSystemComponent::ResetCanSwapTuning, 1, false);
}

//Check if can tune. -1 return to basic tuning. -2 no tuning
int UWeaponsSystemComponent::CanTune(int tuning)
{
	if (!canSwapTuning)
		return -2;

	if (tuning < 0 || equippedTunings.Num() >= tuning || currentTuning == &equippedTunings[tuning])
		return -1;

	return tuning;
}

void UWeaponsSystemComponent::GiveTuningInfo()
{
	int level = FMath::Clamp(currentHeldWeapon->GetWeaponInfo().level, 0, currentTuning->statsTable->GetRowNames().Num());

	FS_Stats* stats = currentTuning->statsTable->FindRow<FS_Stats>(FName(*FString::FromInt(level)), "", true);

	currentHeldWeapon->ChangeWeaponStats(stats);

	player->UpdateWalkSpeeds(stats->speeds);
}
#pragma endregion

#pragma region Resetters
//Resets if tuning can swap
void  UWeaponsSystemComponent::ResetCanSwapTuning()
{
	canSwapTuning = true;
}
#pragma endregion

#pragma region Old
/*
//Get current weapon type
E_WeaponType UWeaponsSystemComponent::GetCurrentWeaponType()
{
	if (currentHeldWeapon)
	{
		return currentHeldWeapon->GetWeaponInfo().weaponType;
	}
	else
	{
		return E_WeaponType::Greatsword;
	}
}

//Swaps weapons
bool UWeaponsSystemComponent::SwapWeapons(AWeaponActor*& swapTo, int whichWeapon)
{
	//Check if can swap weapon
	if (canSwapWeapon)
	{
		//Check if weapon to swap to is not current weapon
		if (swapTo != currentHeldWeapon)
		{
			//Temporarily hold info
			AWeaponActor* tempWeapon = currentHeldWeapon;

			//swap weapons
			currentHeldWeapon = storedWeapon;
			currentHeldWeapon->Activate();
			//change stats
			if (APlayerCharacterCombat* player = Cast<APlayerCharacterCombat>(GetOwner()))
			{
				player->UpdateWalkSpeeds(currentHeldWeapon->GetStats().speeds);
			}

			storedWeapon = tempWeapon;
			storedWeapon->Deactivate();
			canSwapWeapon = false;

			//Timer to hold swap weapons
			GetWorld()->GetTimerManager().SetTimer(swapWeaponsTimer, this, &UWeaponsSystemComponent::ResetCanSwapWeapon, 2, false);

			//Broadcast tuning and weapons
			OnWeaponSwapped.Broadcast(whichWeapon);
			OnTuningSwapped.Broadcast(0);
			OnChangeMusic.Broadcast(static_cast<int32>(currentHeldWeapon->GetWeaponInfo().genre));

			return true;
		}
		return false;
	}

	return false;
}
*/
#pragma endregion