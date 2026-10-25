

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Components/ActorComponent.h"
#include "DamageSystem/DamageInfo.h"
#include "Input/InputActions/CombatActionBase.h"
#include "CombatSystem/Directions.h"
#include "CombatSystemComponent.generated.h"

class APlayerCharacterCombat;
class AMusicManagerCombat;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComboDrop);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboChange, int, currentCombo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboBarChange, float, comboBar);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISSONANCE_ARIA_API UCombatSystemComponent : public UActorComponent
{
	GENERATED_BODY()

private:
#pragma region References
	APlayerCharacterCombat* owner;

	UWorld* world;
#pragma endregion

#pragma region Input Checks
	bool canAttack = true;

	bool canReadInput = true;

	bool isDodging = false;
#pragma endregion

#pragma region Music
	AMusicManagerCombat* musicManager;

	int lastAttackBeat;
#pragma endregion

#pragma region Attacks
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float maxMult = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "0.5"))
	float preBeatLeniency = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "0.5"))
	float postBeatLeniency = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "0.2"))
	float resetTime = 0.05f;

	E_CombatDirections previousDirection = E_CombatDirections::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "0.15"))
	float critRange = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "0.15"))
	float critDamage = 2;

	float musicMult;
#pragma endregion

#pragma region Combo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", meta = (AllowPrivateAccess = "true"))
	int combo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "1"))
	float comboBar = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "0.1"))
	float baseBarFill = 0.025f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", meta = (AllowPrivateAccess = "true"))
	int maxBarMult = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "1"))
	float depletionMult = 0.25;
#pragma endregion

#pragma region Timer
	FTimerHandle resetAttackInfoTimer;

	FTimerHandle readInputTimer;
#pragma endregion

#pragma region Old
	/*
	FTimerHandle combatTimer;

	FTimerHandle queueTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool upSwing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool downSwing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool rightSwing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool leftSwing;

	bool isCharging = false;

	bool didCharge = false;

	bool tickUp = false;

	float powerMult;

	float timeBetweenInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float maxTimeBetweenInput = 2.5;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<float> damageMults = {1, 1.2f, 1.8f};

	TQueue<E_CombatActionType> combatQueue;

	int queueCount;

	TArray<E_CombatActionType> previousActions;
	*/
#pragma endregion

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
#pragma region General/Setup
	UCombatSystemComponent();
#pragma endregion

#pragma region Input
	//Reads player input
	bool ReadInput();

	//Performs the input
	void PerformInput();

	//Resets canReadInput to true
	void ResetReadInputs() { canReadInput = true; }
#pragma endregion

#pragma region Attack
	//Resets all attack info
	void ResetAttackInfo();

	//Returns if player is able to attack
	bool CheckIfValidAttack(float beatPosition);

	//Deals damage to hit actor
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void DealDamage(AActor*& enemyHit, FS_DamageInfo& damageInfo);

	UFUNCTION(BlueprintNativeEvent)
	float MusicalMultiplier();
#pragma endregion

#pragma region Combos
	void RaiseBar(float raiseAmount);

	void DepleteBar(float depleteAmount);
#pragma endregion

#pragma region DownBeat
	UFUNCTION()
	//Perform behaviour of downbeat of music
	void DownBeat();
#pragma endregion

#pragma region Getters
	//Returns the combat direction
	UFUNCTION(BlueprintCallable, BlueprintPure)
	E_CombatDirections GetCombatDirection() { return previousDirection; }
#pragma endregion

#pragma region SetDodging
	UFUNCTION()
	void SetIsDodging(E_Directions directions);
#pragma endregion

#pragma region Delegates
	UPROPERTY(BlueprintAssignable)
	FOnComboDrop OnComboDrop;

	UPROPERTY(BlueprintAssignable)
	FOnComboChange OnComboChange;

	UPROPERTY(BlueprintAssignable)
	FOnComboBarChange OnComboBarChange;
#pragma endregion

#pragma region Old
	/*
	//Adds action type to combat queue
	//void AddToCombatQueue(E_CombatActionType action);

	//Reads the combat queue and performs actions based on it
	//void ReadCombatQueue();

	//bool StartCharge();

	//void Charge();

	//void StopCharge();

	int GetPreviousActionsAmount() { return previousActions.Num(); }

	//Resets the can attack variable to true
	void ResetCanAttack();

	//Resets ability to read inputs
	void ResetReadInputs();

	void QueueCountUp();

	//Emptys the queue and waits to let players attack
	void ResetQueue(float waitTime);


	UFUNCTION(BlueprintCallable)
	void AttackFinished();
	*/
#pragma endregion
};