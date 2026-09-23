

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/WeaponActor.h"
#include "Inventory/InventoryComponent.h"
#include "WeaponsSystemComponent.generated.h"

class AWeaponActor;
class PlayerCharacterCombat;

DECLARE_MULTICAST_DELEGATE(FWeaponsSpawned);

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

	//Set Weapons
	void SpawnWeapons(UInventoryComponent* inventory, APlayerCharacterCombat* player);
};
