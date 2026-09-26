

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayerCharacterBase.h"
#include "BasicFunctions/GetGameInfo.h"
#include "CombatSystem/CombatSystemComponent.h"
#include "CombatSystem/AttacksInterface.h"
#include "Weapons/WeaponActor.h"
#include "Weapons/WeaponsSystemComponent.h"
#include "Inventory/InventoryComponent.h"
#include "DamageSystem/DamageableInterface.h"
#include "PlayerCharacterCombat.generated.h"

class UInputAction;
class UCombatActionBase;

UCLASS()
class DISSONANCE_ARIA_API APlayerCharacterCombat : public APlayerCharacterBase, public IAttacksInterface, public IDamageableInterface
{
	GENERATED_BODY()

private:

#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCombatSystemComponent* combatSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWeaponsSystemComponent* weaponsSystem;

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

	//Getters
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	UCombatSystemComponent* GetCombatSystem() { return combatSystem; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	UWeaponsSystemComponent* GetWeaponsSystem() { return weaponsSystem; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	AWeaponActor* GetWeaponActor(int weapon) { return (weapon == 0) ? weapon1 : weapon2; }

	//Read the light attack input
	void ReadLightAttack();

	//Read the heavy attack input
	void ReadHeavyAttack();

	//Read the side step input
	void ReadSidestep();

	//Implementation of Damageable interface
#pragma region DamageableInterface
	virtual float LightAttack_Implementation(TArray<E_CombatActionType>& previousActions) override;
	virtual float HeavyAttack_Implementation(TArray<E_CombatActionType>& previousActions, bool charged) override;
	virtual float SpecialAttack_Implementation() override;
#pragma
};
