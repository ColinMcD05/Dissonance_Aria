

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/WeaponInfo.h"
#include "Components/CapsuleComponent.h"
#include "DamageSystem/DamageableInterface.h"
#include "CombatSystem/HurtBoxInterface.h"
#include "Input/InputActions/CombatActionBase.h"
#include "Components/WeaponHitbox.h"
#include "WeaponActor.generated.h"

class APlayerCharacterCombat;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponToleranceGone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTuningToleranceGone);

UCLASS()
class DISSONANCE_ARIA_API AWeaponActor : public AActor, public IHurtBoxInterface
{
	GENERATED_BODY()
	
private:
#pragma region References
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* weaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	APlayerCharacterCombat* playerOwner;
#pragma endregion

#pragma region Weapon Info
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	FS_Stats stats;

	bool activated = false;
#pragma endregion

#pragma region Attack Info
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	bool attacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	bool attackCharged = false;

	E_CombatActionType lastAttack;

	TArray<AActor*> hitEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float radius = 30;

	E_Genre currentGenre;
#pragma endregion

#pragma region Tolerance Info
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float depletionAmount = 0.75;

	bool depleted = false;
#pragma endregion

#pragma region Timer
	FTimerHandle disableTimer;

	FTimerHandle raiserTimer;
#pragma endregion
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData", meta = (AllowPrivateAccess = "true"))
	FS_WeaponInfo weaponInfo;

public:

	AWeaponActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region Weapon Creattion
	//Activates the weapon
	void Activate();

	//Deactivates the weapon
	void Deactivate();

	//Initialize weapon on creation
	void InitializeWeapon(APlayerCharacterCombat* player, FS_WeaponInfo& newWeaponInfo);
#pragma endregion

#pragma region Attacking
	//Perform light attack
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Attack")
	float PerformLightAttack(const TArray<E_CombatActionType>& previousActions);

	//Perform heavy attack
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Attack")
	float PerformHeavyAttack(const TArray<E_CombatActionType>& previousActions);

	/*
	//Start charge
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Attack")
	void StartChargeAttack();

	//Charge attack ready
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Attack")
	void ChargeAttack();
	*/

	//Starts the sphere trace for attacking
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartSphereTrace();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PerformSphereTrace();

	//Stops sphere trace
	UFUNCTION(BlueprintCallable)
	void StopSphereTrace() { attacking = false; }
#pragma endregion

#pragma region Weapon Info
	//Gets the weapon info
	UFUNCTION(BlueprintCallable, Category = "WeaponData")
	FS_WeaponInfo& GetWeaponInfo() { return weaponInfo; }

	//Sets weapon info
	UFUNCTION(BlueprintCallable, Category = "WeaponData")
	void SetWeaponInfo(FS_WeaponInfo& newWeaponInfo);

	//Get mac health of weapon
	int GetMaxHealth() { return stats.maxHP; }

	//Changes the weapons stats
	void ChangeWeaponStats(FS_Stats* newStats);

	//Gets stats of weapon
	FS_Stats GetStats() { return stats; }
#pragma endregion

#pragma region EndCombat
	UFUNCTION()
	void CombatEnd(int gainedExp, int gold);
#pragma endregion

#pragma region AttackInterface
	//Interface Functions
	virtual bool OverlappedActor_Implementation(AActor* hitEnemy) override;
#pragma endregion
};
