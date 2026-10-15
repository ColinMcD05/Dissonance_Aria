


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

	//Broadcasts the tuning tolerance
	OnTuningSwapped.AddDynamic(this, &UWeaponsSystemComponent::BroadCastTuningTolerance);
}


// Called every frame
void UWeaponsSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

#pragma region Weapons
//Spawns in the weapons
void UWeaponsSystemComponent::SpawnWeapons(UInventoryComponent* inventory, APlayerCharacterCombat* player)
{
	//Enum to print the weapon string
	const UEnum* Enum = StaticEnum<E_WeaponType>();

	//Collect the inventory weapon
	FS_WeaponInfo* weaponOne = inventory->GetWeaponOne();
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


			FString enumName = Enum->GetDisplayNameTextByValue(static_cast<int64>(currentHeldWeapon->GetWeaponInfo().weaponType)).ToString();

			//Attach to component
			currentHeldWeapon->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(*enumName));
			currentHeldWeapon->Activate();
			player->UpdateWalkSpeeds(currentHeldWeapon->GetStats().speeds);

			OnChangeMusic.Broadcast(static_cast<int32>(currentHeldWeapon->GetWeaponInfo().genre));
		}
	}

	//Get weapon two
	FS_WeaponInfo* weaponTwo = inventory->GetWeaponTwo();
	//Create weapon two
	if (weaponTwo)
	{
		//Create the spawn parameters
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = player;
		spawnParams.Instigator = player->GetInstigator();

		//Spawn and initialize weapons
		if (weaponTwo->weaponActor)
		{
			storedWeapon = GetWorld()->SpawnActor<AWeaponActor>(weaponTwo->weaponActor, player->GetActorTransform(), spawnParams);
			storedWeapon->InitializeWeapon(player, *weaponTwo);

			FString enumName = Enum->GetDisplayNameTextByValue(static_cast<int64>(storedWeapon->GetWeaponInfo().weaponType)).ToString();

			storedWeapon->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(*enumName));
			storedWeapon->Deactivate();
		}
	}

	//Set up UI tolerance bar
	SendCurrentWeaponTolerance();
	//Start timer for allowing tuning
	GetWorld()->GetTimerManager().SetTimer(swapTuningTimer, this, &UWeaponsSystemComponent::ResetCanSwapTuning, 3, false);
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

//Send weapon tolerance
void UWeaponsSystemComponent::SendCurrentWeaponTolerance()
{
	//Get current tolerance
	float weaponTolerance = currentHeldWeapon->GetCurrentWeaponTolerance();

	//Broadcast tolerance
	OnWeaponToleranceChange.Broadcast(weaponTolerance);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UWeaponsSystemComponent::SendCurrentWeaponTolerance);
}

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
#pragma endregion

#pragma region Tuning
//Swap weapon tunings
void UWeaponsSystemComponent::SwapTunings(int tuning)
{
	//Checks if can swap tuning
	if (canSwapTuning && currentHeldWeapon)
	{
		//Second check on if can swap tuning, and which tuning
		int swap = currentHeldWeapon->SwapTuning(tuning);
		//If can swap tuning, start swap
		if (swap != -1)
		{
			canSwapTuning = false;

			//Broadcast music and tuning change
			OnTuningSwapped.Broadcast(swap);
			OnChangeMusic.Broadcast(static_cast<int32>(currentHeldWeapon->GetWeaponInfo().genre) + currentHeldWeapon->GetCurrentTuning());

			//Update walkspeed
			if (APlayerCharacterCombat* player = Cast<APlayerCharacterCombat>(GetOwner()))
			{
				player->UpdateWalkSpeeds(currentHeldWeapon->GetStats().speeds);
			}
		}
		GetWorld()->GetTimerManager().SetTimer(swapTuningTimer, this, &UWeaponsSystemComponent::ResetCanSwapTuning, 2, false);
	}
}

//Check if can tune
int UWeaponsSystemComponent::CanTune(int tuning)
{
	if (!canSwapTuning)
	{
		return -1;
	}
	return currentHeldWeapon->CanTune(tuning);
}


//When returning -1, there is no active tuning
void UWeaponsSystemComponent::SendCurrentTuningTolerance()
{
	//Send tuning tolerance if tuning is active
	if (canBroadcastTolerance)
	{
		OnTuningToleranceChange.Broadcast(currentHeldWeapon->GetCurrentTuningTolerance());
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UWeaponsSystemComponent::SendCurrentTuningTolerance);
	}
}

//Broadcast tuning tolerance
void UWeaponsSystemComponent::BroadCastTuningTolerance(int whichTuning)
{
	//Check if can send tuning tolerance
	if (whichTuning <= 0 )
	{
		canBroadcastTolerance = false;
	}
	else
	{
		if (!canBroadcastTolerance)
		{
			//Start sending tuning tolerance
			canBroadcastTolerance = true;
			SendCurrentTuningTolerance();
		}
	}
}
#pragma endregion

#pragma region Resetters
//Resets if weapon can swap
void UWeaponsSystemComponent::ResetCanSwapWeapon()
{
	canSwapWeapon = true;
}

//Resets if tuning can swap
void  UWeaponsSystemComponent::ResetCanSwapTuning()
{
	canSwapTuning = true;
}
#pragma endregion