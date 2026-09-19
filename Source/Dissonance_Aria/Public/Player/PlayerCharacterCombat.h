

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatSystem/CombatSystemComponent.h"
#include "Weapons/WeaponActor.h"
#include "Weapons/WeaponsSystemComponent.h"
#include "PlayerCharacterCombat.generated.h"

UCLASS()
class DISSONANCE_ARIA_API APlayerCharacterCombat : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCombatSystemComponent* combatSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWeaponsSystemComponent* weaponsSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	AWeaponActor* weapon1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	AWeaponActor* weapon2;

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
};
