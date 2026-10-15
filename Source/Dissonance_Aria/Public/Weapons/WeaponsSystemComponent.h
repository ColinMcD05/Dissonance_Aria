

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/WeaponActor.h"
#include "Inventory/InventoryComponent.h"
#include "Weapons/WeaponInfo.h"
#include "WeaponsSystemComponent.generated.h"

class AWeaponActor;
class PlayerCharacterCombat;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponsSwapped, int, whichWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTuningSwapped, int, whichTuning);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeMusic, int, musicChannel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponToleranceChange, float, newWeaponTolerance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTuningToleranceChange, float, newTuningTolerance);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISSONANCE_ARIA_API UWeaponsSystemComponent : public UActorComponent
{
	GENERATED_BODY()

private:
#pragma region Weapon
	//Weapon references

	//Weapon that's currently held
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	AWeaponActor* currentHeldWeapon;

	//Weapon that's stored for fighting
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	AWeaponActor* storedWeapon;
#pragma endregion
	//Checks for swapping weapons and tunings
	bool canSwapWeapon = true;
	bool canSwapTuning = false;

	//Bool for if tolerance should be broadcasted
	bool canBroadcastTolerance = false;

#pragma region Timers
	//Timers
	FTimerHandle swapWeaponsTimer;
	FTimerHandle swapTuningTimer;
#pragma endregion

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UWeaponsSystemComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region Weapons
	//Spawn and initialize weapon
	void SpawnWeapons(UInventoryComponent* inventory, APlayerCharacterCombat* player);

	//Swap held weapon and stored weapon
	bool SwapWeapons(AWeaponActor*& swapTo, int whichWeapon);

	//Get which weapon is held
	UFUNCTION(BlueprintCallable)
	E_WeaponType GetCurrentWeaponType();

	//Send the tolerance of the current equipped weapon
	UFUNCTION(BlueprintCallable)
	void SendCurrentWeaponTolerance();

	//Getters
	AWeaponActor* GetCurrentHeldWeapon() { return currentHeldWeapon; }

	AWeaponActor* GetStoredWeapon() { return storedWeapon; }
#pragma endregion 

#pragma region Tunings
	//Swap tunings
	void SwapTunings(int tuning);

	//Check if player can tune. -1 if cannot tune, else returns tuning
	int CanTune(int tuning);

	//When returning -1, there is no active tuning
	UFUNCTION(BlueprintCallable)
	void SendCurrentTuningTolerance();

	//Broadcast tuning tolerance changes
	UFUNCTION()
	void BroadCastTuningTolerance(int whichTuning);
#pragma endregion


#pragma region Resetters
	void ResetCanSwapWeapon();

	void ResetCanSwapTuning();
#pragma endregion

#pragma region Delegates
	//Delegate that gets broadcasted once weapons spawn
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FWeaponsSwapped OnWeaponSwapped;

	//Delegate that gets broadcasted once tunings spawn
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FTuningSwapped OnTuningSwapped;

	////Delegate that gets broadcasted once music changes
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnChangeMusic OnChangeMusic;

	//Delegate that gets broadcasted once weapon tolerance changes
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnWeaponToleranceChange OnWeaponToleranceChange;

	//Delegate that gets broadcasted once tuning tolerance changes
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnTuningToleranceChange OnTuningToleranceChange;
#pragma endregion
};
