

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/WeaponActor.h"
#include "Inventory/InventoryComponent.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	AWeaponActor* currentHeldWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	AWeaponActor* storedWeapon;
#pragma endregion

	bool canSwapWeapon = true;
	bool canSwapTuning = true;
	bool canBroadcastTolerance = false;
	FTimerHandle swapWeaponsTimer;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UWeaponsSystemComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Getters
	AWeaponActor* GetCurrentHeldWeapon() { return currentHeldWeapon; }
	AWeaponActor* GetStoredWeapon() { return storedWeapon; }

	//Set/Swap Weapons
	void SpawnWeapons(UInventoryComponent* inventory, APlayerCharacterCombat* player);
	bool SwapWeapons(AWeaponActor*& swapTo, int whichWeapon);
	void ResetCanSwapWeapon();
	UFUNCTION(BlueprintCallable)
	void SendCurrentWeaponTolerance();

	//Swap tunings
	void SwapTunings(int tuning);
	void ResetCanSwapTuning();
	int CanTune(int tuning);
	//When returning -1, there is no active tuning
	UFUNCTION(BlueprintCallable)
	void SendCurrentTuningTolerance();
	void BroadCastTuningTolerance(int whichTuning);

#pragma region Delegates
	//Delegate that gets broadcasted once weapons spawn
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FWeaponsSwapped OnWeaponSwapped;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FTuningSwapped OnTuningSwapped;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnChangeMusic OnChangeMusic;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnWeaponToleranceChange OnWeaponToleranceChange;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnTuningToleranceChange OnTuningToleranceChange;
#pragma endregion
};
