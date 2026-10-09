


#include "CombatSystem/CombatSystemComponent.h"
#include "Player/PlayerCharacterCombat.h"

// Sets default values for this component's properties
UCombatSystemComponent::UCombatSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UCombatSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	//Assigns owner for easier access
	owner = Cast<APlayerCharacterCombat>(GetOwner());

	//Gets world for easier access
	if (owner)
	{
		world = owner->GetWorld();
	}
	
}


// Called every frame
void UCombatSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//Adds action type to combat queue
void UCombatSystemComponent::AddToCombatQueue(E_CombatActionType action)
{
	//Checks if can add input, then add input
	if (canReadInput)
	{
		if (queueCount < MAX_COMBO_LENGTH)
		{
			combatQueue.Enqueue(action);
			queueCount++;

			//Read input
			ReadCombatQueue();
			if (owner)
			{
				musicMults.Add(owner->MusicalMultiplier());
			}

			canReadInput = false;
			world->GetTimerManager().SetTimer(queueTimer, this, &UCombatSystemComponent::ResetReadInputsOnly, 0.2f, false);
		}
	}
}

//Reads the combat queue and performs actions based on it
void UCombatSystemComponent::ReadCombatQueue()
{
	//Checks if the player is currently able to attack
	if (!canAttack)
	{
		//Ensures that the timer has started or not. If it hasn't, start the timer.
		if (world->GetTimerManager().IsTimerActive(combatTimer))
		{
			return;
		}
		world->GetTimerManager().SetTimer(combatTimer, this, &UCombatSystemComponent::ResetCanAttack, 0.1f, false);

		return;
	}

	//Check if the combat queue is empty
	if (combatQueue.IsEmpty())
	{
		return;
	}

	//Inizialize some variables
	IAttacksInterface* attacker = Cast<IAttacksInterface>(owner);
	float waitTime;
	E_CombatActionType nextAction;
	combatQueue.Dequeue(nextAction);

	//Looks at the tail of queue and perform correlated actions. Then Pop
	switch (nextAction)
	{
		case E_CombatActionType::HeavyAttack:
			
			if (attacker)
			{
				waitTime = attacker->Execute_HeavyAttack(Cast<UObject>(attacker), previousActions, false);
				if (previousActions.IsEmpty())
				{
					ResetQueue(waitTime+0.5f);
					return;
				}
				previousActions.Add(E_CombatActionType::HeavyAttack);
			}
			break;
		case E_CombatActionType::LightAttack:
			if (attacker)
			{
				waitTime = attacker->Execute_LightAttack(Cast<UObject>(attacker), previousActions);
				previousActions.Add(E_CombatActionType::LightAttack);	
			}
			break;
	}

	//Ensures players can no longer attack
	canAttack = false;

	if (previousActions.Num() >= MAX_COMBO_LENGTH || didCharge)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, TEXT("Resseting"));
		}
		canReadInput = false;
		didCharge = false;
		world->GetTimerManager().ClearTimer(readInputOnlyTimer);
		world->GetTimerManager().SetTimer(queueTimer, this, &UCombatSystemComponent::ResetReadInputs, waitTime + 0.5f, false);
	}
	else
	{
		//Start the timer
		world->GetTimerManager().SetTimer(combatTimer, this, &UCombatSystemComponent::ResetCanAttack, waitTime + 0.1f, false);
	}
}

bool UCombatSystemComponent::StartCharge()
{
	if(canReadInput && canAttack)
	{
		powerMult = 1;
		canReadInput = false;
		canAttack = false;
		isCharging = true;
		didCharge = true;
		return true;
	}
	return false;
}

void UCombatSystemComponent::Charge()
{
	if (isCharging && maxMult > powerMult)
	{
		powerMult += GetWorld()->DeltaTimeSeconds / 2;
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UCombatSystemComponent::Charge);
	}
	else if (!isCharging)
	{
		canAttack = true;
		canReadInput = true;
		AddToCombatQueue(E_CombatActionType::HeavyAttack);
	}
}

void UCombatSystemComponent::StopCharge()
{
	if (isCharging)
	{
		isCharging = false;
	}
}

//Deals damage to hit actor
void UCombatSystemComponent::DealDamage(AActor*& actorHit,  FS_DamageInfo& damageInfo)
{
	//Gets damage actor and execute TakeDamage
	if (!actorHit || !actorHit->Implements<UDamageableInterface>())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Can't damage"));
		}
		return;
	}

	FS_DamageInfo changedDamage = damageInfo;

	changedDamage.damageAmount *= powerMult;
	if (musicMults.Num() >= queueCount)
	{
		changedDamage.damageAmount *= musicMults[queueCount - 1];
	}
	if (damageMults.Num() >= queueCount)
	{
		changedDamage.damageAmount *= damageMults[queueCount - 1];
	}

	IDamageableInterface::Execute_TakeDamage(actorHit, damageInfo);
}

//Resets the can attack variable to true
void UCombatSystemComponent::ResetCanAttack()
{
	//Lets player attack and recall read combat
	canAttack = true;
	ReadCombatQueue();
}

/*Emptys the queue and waits to let players attack
* @param waitTime The time the animation takes to play before player can attack again 
*/ 
void UCombatSystemComponent::ResetQueue(float waitTime)
{
	//Clears timer
	world->GetTimerManager().ClearTimer(combatTimer);

	//Prevents player from attack of inpit reading
	canReadInput = false;
	canAttack = false;

	//Empty queue
	combatQueue.Empty();
	queueCount = 0;
	previousActions.Empty();
	powerMult = 1;

	musicMults.Empty();

	//Sets timer
	world->GetTimerManager().SetTimer(queueTimer, this, &UCombatSystemComponent::ResetReadInputs, waitTime, false);
}

//Resets ability to read inputs
void UCombatSystemComponent::ResetReadInputs()
{
	//Allows player to attack
	canReadInput = true;
	canAttack = true;
	didCharge = false;

	//Empty queue
	combatQueue.Empty();
	queueCount = 0;
	previousActions.Empty();
	powerMult = 1;
	musicMults.Empty();
}

void UCombatSystemComponent::QueueCountUp()
{
	if (timeBetweenInput < maxTimeBetweenInput)
	{
		timeBetweenInput += world->DeltaTimeSeconds;
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UCombatSystemComponent::QueueCountUp);
	}
	else
	{
		ResetQueue(0.1f);
	}
}