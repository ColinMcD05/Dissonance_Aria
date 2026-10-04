

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayerCharacterBase.h"
#include "BasicFunctions/GetGameInfo.h"
#include "CombatSystem/CombatSystemComponent.h"
#include "CombatSystem/AttacksInterface.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapons/WeaponActor.h"
#include "Weapons/WeaponsSystemComponent.h"
#include "Inventory/InventoryComponent.h"
#include "DamageSystem/DamageableInterface.h"
#include "DamageSystem/DamageSystemComponent.h"
#include "GameInfo/DAGameStateCombat.h"
#include "PlayerCharacterCombat.generated.h"

class UInputAction;
class UCombatActionBase;

UCLASS()
class DISSONANCE_ARIA_API APlayerCharacterCombat : public APlayerCharacterBase, public IAttacksInterface, public IDamageableInterface
{
	GENERATED_BODY()

private:

	//Needed components and actors
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCombatSystemComponent* combatSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWeaponsSystemComponent* weaponsSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UDamageSystemComponent* damageSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* weaponSpot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	AWeaponActor* weapon1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	AWeaponActor* weapon2;
#pragma endregion

#pragma region Inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
	UCombatActionBase* lightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
	UCombatActionBase* heavyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
	UInputAction* sideStep;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
	UInputAction* swapWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
	UInputAction* changeLockon;
#pragma endregion

#pragma region Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* springArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	bool cameraMoveLag = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	bool cameraRotationLag = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float distanceFromPlayer = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float rotateSpeed = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	AActor* lockedOnEnemy;

	int enemyIndex = -1;
#pragma endregion

	FTimerHandle resetLevelTimer;

#pragma region GameInfo
	ADAGameStateCombat* gameState;
#pragma endregion
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	APlayerCharacterCombat();

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpdateInventoryWeaponInfo(FS_WeaponInfo updatedInfo);

	void ResetCurrentLevel();

#pragma region Getters
	//Get the combat system
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	UCombatSystemComponent* GetCombatSystem() { return combatSystem; }

	//Get the weapons system
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	UWeaponsSystemComponent* GetWeaponsSystem() { return weaponsSystem; }

	//Get weapon actor based on index. 0 for weapon 1, 1 for weapon 2
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	AWeaponActor* GetWeaponActor(int weapon) { return (weapon == 0) ? weapon1 : weapon2; }

	//Get where weapons should spawn
	USceneComponent* GetWeaponSpot() { return weaponSpot; }
#pragma endregion

#pragma region Input
	//Read the light attack input
	void ReadLightAttack();

	//Read the heavy attack input
	void ReadHeavyAttack();

	//Read the side step input
	void ReadSidestep();

	//Read Swap Weapon input
	void ReadSwapWeapon(const FInputActionValue& value);

	//Read the change lock on input
	void ReadChangeLockOn(const FInputActionValue& value);
#pragma endregion

#pragma region Camera
	UFUNCTION()
	//Relook for a valid enemy when one dies
	void CameraEnemySearch(AActor* newEnemy);

	//Focus the camera on enemy
	void FocusOnEnemy();

	//Rotate the player to face enemy
	void RotatePlayer();
#pragma endregion
	//Implementation of Attack interface
#pragma region AttackInterface
	virtual float LightAttack_Implementation(TArray<E_CombatActionType>& previousActions) override;
	virtual float HeavyAttack_Implementation(TArray<E_CombatActionType>& previousActions, bool charged) override;
	virtual float SpecialAttack_Implementation() override;
#pragma

#pragma region DamageableInterface
	virtual float GetCurrentHealth_Implementation() override;
	virtual float GetMaxHealth_Implementation() override;
	virtual bool GetIsDead_Implementation() override;
	virtual void Heal_Implementation(float HealAmount, AActor* Healer) override;
	virtual bool TakeDamage_Implementation(FS_DamageInfo DamageInfo) override;
	virtual void HandleDeath_Implementation(AActor* killer) override;
#pragma
};
