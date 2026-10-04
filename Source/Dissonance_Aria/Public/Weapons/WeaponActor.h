

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

UCLASS()
class DISSONANCE_ARIA_API AWeaponActor : public AActor, public IHurtBoxInterface
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* weaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TArray<UWeaponHitbox*> hurtboxes;

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

	void EnableHurtboxes();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Attack")
	void DisableHurtboxes();

	int GetMaxHealth() { return stats.maxHP; }

	int CalculateAnimationPosition(const TArray<E_CombatActionType>& previousActions, E_CombatActionType currentAction);

	void CombatEnd(int gainedExp);

	void ChangeWeaponStats();

	void LowerTolerance();

	void RaiseTolerance();
};
