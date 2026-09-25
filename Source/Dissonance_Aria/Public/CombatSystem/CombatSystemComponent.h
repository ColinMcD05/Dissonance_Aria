

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Components/ActorComponent.h"
#include "DamageSystem/DamageInfo.h"
#include "Input/InputActions/CombatActionBase.h"
#include "CombatSystemComponent.generated.h"

#define MAX_COMBO_LENGTH 3

class APlayerCharacterCombat;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISSONANCE_ARIA_API UCombatSystemComponent : public UActorComponent
{
	GENERATED_BODY()

private:
#pragma region Input
	TQueue<E_CombatActionType> combatQueue;

	int queueCount;
#pragma endregion

#pragma region References
	AActor* owner;

	UWorld* world;
#pragma endregion
	bool canAttack = true;

	bool canReadInput = true;

#pragma region Timer
	FTimerHandle combatTimer;

	FTimerHandle queueTimer;
#pragma endregion

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UCombatSystemComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Adds action type to combat queue
	void AddToCombatQueue(E_CombatActionType action);

	//Deals damage to hit actor
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void DealDamage(AActor*& enemyHit, FS_DamageInfo& damageInfo);

	//Reads the combat queue and performs actions based on it
	void ReadCombatQueue();

	//Resets the can attack variable to true
	void ResetCanAttack();

	//Emptys the queue and waits to let players attack
	void ResetQueue(float waitTime);

	//Resets ability to read inputs
	void ResetReadInputs();
};
