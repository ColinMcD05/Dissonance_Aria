

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

	FVector originalLocation;
	float speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* weaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponData", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	int currentTuning = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	FS_Stats stats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponData", meta = (AllowPrivateAccess = "true"))
	TArray<UDataTable*> statsTable;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	APlayerCharacterCombat* playerOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float timeToDrainTolerance = 90;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float timeToFillTolerance = 45;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float timeToDrainTuning = 35;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float timeToFillTuning = 25;

	bool raisingTolerance = false;

	bool depleted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float depletionAmount = 0.75;

	FTimerHandle disableTimer;

	FTimerHandle raiserTimer;

	bool activated = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData", meta = (AllowPrivateAccess = "true"))
	FS_WeaponInfo weaponInfo;

public:

	AWeaponActor();

	void Activate();

	void Deactivate();

	void InitializeWeapon(APlayerCharacterCombat* player, FS_WeaponInfo& newWeaponInfo);

	//Interface Functions
	virtual bool OverlappedActor_Implementation(AActor* hitEnemy) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "WeaponData")
	FS_WeaponInfo& GetWeaponInfo() { return weaponInfo; }

	UFUNCTION(BlueprintCallable, Category = "WeaponData")
	void SetWeaponInfo(FS_WeaponInfo& newWeaponInfo);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Attack")
	float PerformLightAttack(const TArray<E_CombatActionType>& previousActions);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Attack")
	float PerformHeavyAttack(const TArray<E_CombatActionType>& previousActions);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Attack")
	void StartChargeAttack();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Attack")
	void ChargeAttack();

	int GetMaxHealth() { return stats.maxHP; }

	int CalculateAnimationPosition(const TArray<E_CombatActionType>& previousActions, E_CombatActionType currentAction);

	UFUNCTION()
	void CombatEnd(int gainedExp);

	void ChangeWeaponStats();

	void LowerTolerance();

	void RaiseTolerance();

	void RaiseTuningTolerance();

	//return -1 if failed. Else, rtuen current tuning
	int SwapTuning(int newTuning);
	int CanTune(int tuning);

	UFUNCTION(BlueprintCallable)
	int GetCurrentTuning();

	float GetCurrentWeaponTolerance();
	//When returning -1, there is no active tuning
	float GetCurrentTuningTolerance();
};
