

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageSystem/DamageInfo.h"
#include "Music/MusicInfo.h"
#include "DamageSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTakeDamage, float, newHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, newMax, float, newCurrent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISSONANCE_ARIA_API UDamageSystemComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float currentHealth = maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	bool isDead = false;

	UPROPERTY(EditAnywhere, Category = "Genre")
	E_Genre favoriteGenre = E_Genre::None;

	UPROPERTY(EditAnywhere, Category = "Genre", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float favortieResistance;

	UPROPERTY(EditAnywhere, Category = "Genre")
	E_Genre hatedGenre = E_Genre::None;

	UPROPERTY(EditAnywhere, Category = "Genre", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float hatedBonus;

	UPROPERTY(EditAnywhere, Category = "Stats", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float defense;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float maxHealth = 100;

	// Sets default values for this component's properties
	UDamageSystemComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	bool HandleIncomingDamage(FS_DamageInfo damageInfo, int& damageAmount);

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void HandleIncomingHeal(float healAmount, AActor* healer);

	// Getters
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	float GetCurrentHealth() const { return currentHealth; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	float GetMaxHealth() const { return maxHealth;  }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	bool GetIsDead() const { return isDead; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetMaxHealthAndCurrent(int newMaxHealth);

#pragma region Delegates
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnTakeDamage OnTakeDamage;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnHealthChange OnHealthChange;
#pragma endregion
};
