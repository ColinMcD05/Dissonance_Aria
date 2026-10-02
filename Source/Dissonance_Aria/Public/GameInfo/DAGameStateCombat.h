

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DAGameStateCombat.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDeath);

UCLASS()
class DISSONANCE_ARIA_API ADAGameStateCombat : public AGameStateBase
{
	GENERATED_BODY()
	
private:
	int totalEnemyExp;

	TArray<UObject*> enemies;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemies")
	void AddEnemies(UObject* newEnemy);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Enemies")
	void IncreaseTotalExp(UObject* newEnemy);

#pragma region Delegates
	UPROPERTY(BlueprintAssignable)
	FOnEnemyDeath OnEnemyDeath;
#pragma endregion

#pragma region Getters
	//Get next enemy
	UFUNCTION(BlueprintCallable, Category = "Enemies")
	UObject* GetNextEnemy(int& currentIndex);

	//Get previous enemy
	UFUNCTION(BlueprintCallable, Category = "Enemies")
	UObject* GetPreviousEnemy(int& currentIndex);
#pragma endregion
};