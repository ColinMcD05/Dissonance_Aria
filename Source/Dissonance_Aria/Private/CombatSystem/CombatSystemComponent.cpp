


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
	owner = GetOwner();

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

	// ...
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
		}

		//Read input
		ReadCombatQueue();
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
		world->GetTimerManager().SetTimer(combatTimer, this, &UCombatSystemComponent::ResetCanAttack, false);

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
				previousActions.Add(E_CombatActionType::HeavyAttack);
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Heavy"));
				}
			}
			break;
		case E_CombatActionType::LightAttack:
			if (attacker)
			{
				waitTime = attacker->Execute_LightAttack(Cast<UObject>(attacker), previousActions);
				previousActions.Add(E_CombatActionType::LightAttack);	
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Light."));
				}
			}
			break;
	}
	//Start the timer
	world->GetTimerManager().SetTimer(combatTimer, this, &UCombatSystemComponent::ResetCanAttack, waitTime, false);

	//Ensures players can no longer attack
	canAttack = false;
}

//Deals damage to hit actor
void UCombatSystemComponent::DealDamage(AActor*& actorHit,  FS_DamageInfo& damageInfo)
{
	//Gets damage actor and execute TakeDamage
	IDamageableInterface* damageActor = Cast<IDamageableInterface>(actorHit);
	damageActor->Execute_TakeDamage(Cast<UObject>(this), damageInfo);
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
	previousActions.Empty();

	//Sets timer
	world->GetTimerManager().SetTimer(queueTimer, this, &UCombatSystemComponent::ResetReadInputs, waitTime, false);
}

//Resets ability to read inputs
void UCombatSystemComponent::ResetReadInputs()
{
	//Allows player to attack
	canReadInput = true;
	canAttack = true;
}