

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience", meta = (AllowPrivateAccess = "true"))
	int totalEnemyExp;

	TArray<AActor*> enemies;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemies")
	void AddEnemies(AActor* newEnemy);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Enemies")
	void IncreaseTotalExp(AActor* newEnemy);

#pragma region Delegates
	UPROPERTY(BlueprintAssignable)
	FOnEnemyDeath OnEnemyDeath;
#pragma endregion

#pragma region Getters
	//Get next enemy
	UFUNCTION(BlueprintCallable, Category = "Enemies")
	AActor* GetNextEnemy(int& currentIndex);

	//Get previous enemy
	UFUNCTION(BlueprintCallable, Category = "Enemies")
	AActor* GetPreviousEnemy(int& currentIndex);
#pragma endregion
};