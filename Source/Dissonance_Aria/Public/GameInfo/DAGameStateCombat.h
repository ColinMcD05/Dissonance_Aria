

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DAGameStateCombat.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeath, AActor*, enemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemySpawned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatEnd, int, gainedExp);

UCLASS()
class DISSONANCE_ARIA_API ADAGameStateCombat : public AGameStateBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience", meta = (AllowPrivateAccess = "true"))
	int totalEnemyExp;

	TArray<AActor*> enemies;

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemies")
	void AddEnemies(AActor* newEnemy);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Enemies")
	void IncreaseTotalExp(AActor* newEnemy);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemies")
	void EnemyDead(AActor* newEnemy);

#pragma region Delegates
	UPROPERTY(BlueprintAssignable)
	FOnEnemyDeath OnEnemyDeath;

	UPROPERTY(BlueprintAssignable)
	FOnEnemySpawned OnEnemySpawned;

	UPROPERTY(BlueprintAssignable)
	FOnCombatEnd OnCombatEnd;
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