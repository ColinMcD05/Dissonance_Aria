

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Components/ActorComponent.h"
#include "DamageSystem/DamageInfo.h"
#include "Input/InputActions/CombatActionBase.h"
#include "CombatSystem/AttackDirections.h"
#include "CombatSystemComponent.generated.h"

#define MAX_COMBO_LENGTH 3

class APlayerCharacterCombat;
class AMusicManagerCombat;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISSONANCE_ARIA_API UCombatSystemComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<float> damageMults = {1, 1.2f, 1.8f};

	TArray<float> musicMults;

#pragma region Music
	AMusicManagerCombat* musicManager;

	int lastAttackBeat;
#pragma endregion

#pragma region Input
	TQueue<E_CombatActionType> combatQueue;

	int queueCount;

	TArray<E_CombatActionType> previousActions;
#pragma endregion

#pragma region References
	APlayerCharacterCombat* owner;

	UWorld* world;
#pragma endregion
	bool canAttack = true;

	bool canReadInput = true;

	bool isCharging = false;

	bool didCharge = false;

	bool tickUp = false;

	float powerMult;

#pragma region Attacks
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool upSwing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool downSwing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool rightSwing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool leftSwing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float maxMult = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "0.5"))
	float preBeatLeniency = 0.25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "0.5"))
	float postBeatLeniency = 0.25;

	float timeBetweenInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float maxTimeBetweenInput = 2.5;

	E_CombatDirections previousDirection;
#pragma endregion

#pragma region Timer
	FTimerHandle combatTimer;

	FTimerHandle queueTimer;

	FTimerHandle readInputOnlyTimer;

	FTimerHandle resetInputsTimer;
#pragma endregion

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UCombatSystemComponent();

	//Deals damage to hit actor
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void DealDamage(AActor*& enemyHit, FS_DamageInfo& damageInfo);

	bool ReadInput();

	void PerformInput();

	void ResetInputs();

	//Resets the can attack variable to true
	void ResetCanAttack();

	//Emptys the queue and waits to let players attack
	void ResetQueue(float waitTime);

	UFUNCTION(BlueprintCallable)
	void AttackFinished();

	//Resets ability to read inputs
	void ResetReadInputs();

	void ResetReadInputsOnly() { canReadInput = true; }

	void QueueCountUp();

	void DownBeat();

	int GetPreviousActionsAmount() { return previousActions.Num(); }

	bool CheckIfValidAttack();
#pragma region Old
	//Adds action type to combat queue
	//void AddToCombatQueue(E_CombatActionType action);

	//Reads the combat queue and performs actions based on it
	//void ReadCombatQueue();

	//bool StartCharge();

	//void Charge();

	//void StopCharge();
#pragma endregion
};