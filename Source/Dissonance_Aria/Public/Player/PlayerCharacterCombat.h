

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
#include "Input/ControllerTypeInfo.h"
#include "CombatSystem/Directions.h"
#include "PlayerCharacterCombat.generated.h"

class UInputAction;
class UCombatActionBase;
class UDodgeComponent;
class UMusicManagerCombat;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerSetUpDone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwapController, E_ControllerType, newController);

UCLASS()
class DISSONANCE_ARIA_API APlayerCharacterCombat : public APlayerCharacterBase, public IAttacksInterface, public IDamageableInterface
{
	GENERATED_BODY()

private:
#pragma region Movement
	bool canMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float snapDistance = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float paddingDistance = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	E_Directions direction;
#pragma endregion

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
	AWeaponActor* weapon1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	AWeaponActor* weapon2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UDodgeComponent* dodgeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	AMusicManagerCombat* musicManager;
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
	UInputAction* swapTuning;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameStateInfo", meta = (AllowPrivateAccess = "true"))
	bool combatEnd = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameStateInfo", meta = (AllowPrivateAccess = "true"))
	E_ControllerType controllerType = E_ControllerType::Keyboard;
#pragma endregion
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	APlayerCharacterCombat();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpdateInventoryWeaponInfo(FS_WeaponInfo updatedInfo, int maxLevel);

	void ResetCurrentLevel();

	UFUNCTION()
	void CombatEnded(int exp);

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

	//Gets the current controller type being used
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	E_ControllerType GetControllerType() { return controllerType; }
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

	//Read Swap Tuning input
	void ReadSwapTuning(const FInputActionValue& value);

	//Read the change lock on input
	void ReadChangeLockOn(const FInputActionValue& value);

	//Read end combat
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void ReadAnyKey(FKey pressedKey);
#pragma endregion

	E_ControllerType KeyboardOrGamepad(FKey pressedKey);

#pragma region Movement
	virtual void Move_Implementation(const FInputActionValue& value) override;

	//Lets the player move
	UFUNCTION()
	void ResetCanMove() { canMove = true; }

	UFUNCTION()
	void ListenToDodge(E_Directions moveDirection);

	//Snaps to the focused enemy
	void SnapToEnemy();

	//Calculates the direction the player is going
	E_Directions CalculateDirection(FVector vector2);
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

	//Logic specifically for heavy attacks
#pragma region HeavyAttack
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Components")
	void StartChargedAttack();

	void ChargeReady();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Components")
	void StopChargedAttack();
#pragma endregion

#pragma region Tuning
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Tunings")
	void StartTuning(float value);

	UFUNCTION(BlueprintCallable, Category = "Tunings")
	void ReadSuccess(bool successful, int tuning);
#pragma

#pragma region SetUp
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnPlayerSetUpDone OnPlayerSetUpDone;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnSwapController OnSwapController;
#pragma

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
