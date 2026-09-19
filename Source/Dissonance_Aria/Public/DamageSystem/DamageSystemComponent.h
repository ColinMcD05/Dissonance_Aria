

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageSystem/DamageInfo.h"
#include "Music/MusicInfo.h"
#include "DamageSystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISSONANCE_ARIA_API UDamageSystemComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	float currentHealth = maxHealth;

	UPROPERTY()
	bool isDead = false;

	UPROPERTY(EditAnywhere, Category = "Genre")
	EGenre favoriteGenre = EGenre::None;

	UPROPERTY(EditAnywhere, Category = "Genre", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float damagePercentageLost;

	UPROPERTY(EditAnywhere, Category = "Genre")
	EGenre hatedGenre = EGenre::None;

	UPROPERTY(EditAnywhere, Category = "Genre", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float damagePercentageGained;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float maxHealth = 100;

	// Sets default values for this component's properties
	UDamageSystemComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	bool HandleIncomingDamage(FS_DamageInfo& damageInfo);

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void HandleIncomingHeal(float healAmount, AActor* healer);

	// Getters
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	float GetCurrentHealth() const { return currentHealth; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	float GetMaxHealth() const { return maxHealth;  }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	bool GetIsDead() const { return isDead; }
};
