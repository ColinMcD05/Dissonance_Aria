


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

	OnTuningSwapped.AddDynamic(this, &UWeaponsSystemComponent::BroadCastTuningTolerance);
}


// Called every frame
void UWeaponsSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UWeaponsSystemComponent::SpawnWeapons(UInventoryComponent* inventory, APlayerCharacterCombat* player)
{
	const UEnum* Enum = StaticEnum<E_WeaponType>();

	FS_WeaponInfo* weaponOne = inventory->GetWeaponOne();
	if (weaponOne)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = player;
		spawnParams.Instigator = player->GetInstigator();

		if (weaponOne->weaponActor)
		{
			currentHeldWeapon = GetWorld()->SpawnActor<AWeaponActor>(weaponOne->weaponActor, player->GetActorTransform(), spawnParams);
			currentHeldWeapon->InitializeWeapon(player, *weaponOne);


			FString enumName = Enum->GetDisplayNameTextByValue(static_cast<int64>(currentHeldWeapon->GetWeaponInfo().weaponType)).ToString();

			currentHeldWeapon->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(*enumName));
			currentHeldWeapon->Activate();
			player->UpdateWalkSpeeds(currentHeldWeapon->GetStats().speeds);
		}
	}

	FS_WeaponInfo* weaponTwo = inventory->GetWeaponTwo();
	if (weaponTwo)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = player;
		spawnParams.Instigator = player->GetInstigator();

		storedWeapon = GetWorld()->SpawnActor<AWeaponActor>(weaponTwo->weaponActor, player->GetActorTransform(), spawnParams);
		storedWeapon->InitializeWeapon(player, *weaponTwo);

		FString enumName = Enum->GetDisplayNameTextByValue(static_cast<int64>(storedWeapon->GetWeaponInfo().weaponType)).ToString();

		storedWeapon->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(*enumName));
		storedWeapon->Deactivate();
	}

	SendCurrentWeaponTolerance();
	GetWorld()->GetTimerManager().SetTimer(swapTuningTimer, this, &UWeaponsSystemComponent::ResetCanSwapTuning, 3, false);
}


bool UWeaponsSystemComponent::SwapWeapons(AWeaponActor*& swapTo, int whichWeapon)
{
	if (canSwapWeapon)
	{
		if (swapTo != currentHeldWeapon)
		{

			AWeaponActor* tempWeapon = currentHeldWeapon;

			currentHeldWeapon = storedWeapon;
			currentHeldWeapon->Activate();
			if (APlayerCharacterCombat* player = Cast<APlayerCharacterCombat>(GetOwner()))
			{
				player->UpdateWalkSpeeds(currentHeldWeapon->GetStats().speeds);
			}

			storedWeapon = tempWeapon;
			storedWeapon->Deactivate();
			canSwapWeapon = false;

			GetWorld()->GetTimerManager().SetTimer(swapWeaponsTimer, this, &UWeaponsSystemComponent::ResetCanSwapWeapon, 2, false);

			OnWeaponSwapped.Broadcast(whichWeapon);
			OnTuningSwapped.Broadcast(0);
			OnChangeMusic.Broadcast(static_cast<int32>(currentHeldWeapon->GetWeaponInfo().genre));

			return true;
		}
		return false;
	}

	return false;
}

void UWeaponsSystemComponent::SendCurrentWeaponTolerance()
{
	float weaponTolerance = currentHeldWeapon->GetCurrentWeaponTolerance();

	OnWeaponToleranceChange.Broadcast(weaponTolerance);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UWeaponsSystemComponent::SendCurrentWeaponTolerance);
}

void UWeaponsSystemComponent::SwapTunings(int tuning)
{
	if (canSwapTuning && currentHeldWeapon)
	{
		int swap = currentHeldWeapon->SwapTuning(tuning);
		if (swap != -1)
		{
			canSwapTuning = false;

			OnTuningSwapped.Broadcast(swap);
			OnChangeMusic.Broadcast(static_cast<int32>(currentHeldWeapon->GetWeaponInfo().genre) + currentHeldWeapon->GetCurrentTuning());
			if (APlayerCharacterCombat* player = Cast<APlayerCharacterCombat>(GetOwner()))
			{
				player->UpdateWalkSpeeds(currentHeldWeapon->GetStats().speeds);
			}
		}
		GetWorld()->GetTimerManager().SetTimer(swapTuningTimer, this, &UWeaponsSystemComponent::ResetCanSwapTuning, 2, false);
	}
	else 
	{
	}
}


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
	if (canBroadcastTolerance)
	{
		OnTuningToleranceChange.Broadcast(currentHeldWeapon->GetCurrentTuningTolerance());
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UWeaponsSystemComponent::SendCurrentTuningTolerance);
	}
}

void UWeaponsSystemComponent::BroadCastTuningTolerance(int whichTuning)
{
	if (whichTuning <= 0 )
	{
		canBroadcastTolerance = false;
	}
	else
	{
		if (!canBroadcastTolerance)
		{
			canBroadcastTolerance = true;
			SendCurrentTuningTolerance();
		}
	}
}

void UWeaponsSystemComponent::ResetCanSwapWeapon()
{
	canSwapWeapon = true;
}

void  UWeaponsSystemComponent::ResetCanSwapTuning()
{
	canSwapTuning = true;
}

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