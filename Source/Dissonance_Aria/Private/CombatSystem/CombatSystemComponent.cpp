


#include "CombatSystem/CombatSystemComponent.h"
#include "Player/PlayerCharacterCombat.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Music/MusicManagerCombat.h"

#pragma region General/Setup
// Sets default values for this component's properties
UCombatSystemComponent::UCombatSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
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
	
	musicManager = Cast<AMusicManagerCombat>(UGameplayStatics::GetActorOfClass(GetWorld(), AMusicManagerCombat::StaticClass()));

	if (musicManager)
	{
		musicManager->OnBeat.AddDynamic(this, &UCombatSystemComponent::DownBeat);
	}
}
#pragma endregion

#pragma region Input
//Reads player input
bool UCombatSystemComponent::ReadInput()
{
	if (!canReadInput)
		return false;

	//Resets timer incase it was running
	GetWorld()->GetTimerManager().ClearTimer(resetAttackInfoTimer);

	//Get how close to beat input was
	float beatPosition = musicManager->GetHowCloseToBeat();

	//Validates attack input
	if (!CheckIfValidAttack(beatPosition))
		return false;

	//Performs input
	PerformInput();

	//Set read input
	canReadInput = false;
	GetWorld()->GetTimerManager().SetTimer(readInputTimer, this, &UCombatSystemComponent::ResetReadInputs, 0.01f);

	return true;
}

//Performs the input
void UCombatSystemComponent::PerformInput()
{
	int randomInt = 0;

	//Get random new attack direction 
	do
	{
		randomInt = UKismetMathLibrary::RandomInteger(4);
	} while (randomInt == static_cast<uint8>(previousDirection));

	//Perform new combat directions
	switch (randomInt)
	{
		case 0:
			previousDirection = E_CombatDirections::Up;
			break;
		case 1:
			previousDirection = E_CombatDirections::Down;
			break;
		case 2:
			previousDirection = E_CombatDirections::Right;
			break;
		case 3:
			previousDirection = E_CombatDirections::Left;
			break;
	}

	musicMult = MusicalMultiplier();

	combo++;
}
#pragma endregion

#pragma region Attack
//Resets attack intfo
void UCombatSystemComponent::ResetAttackInfo()
{
	//Sets previous direction to none
	previousDirection = E_CombatDirections::None;

	//Sets combo to 0
	combo = 0;
}

//Returns if the player can attack this turn
bool UCombatSystemComponent::CheckIfValidAttack(float beatPosition)
{
	//Check if music manager exits
	if (musicManager == nullptr)
		return false;

	//If currently has attacked this beat, do not allow another attack
	if (lastAttackBeat == musicManager->GetFocusedBeat())
	{
		return false;
	}

	//Check song position
	if (beatPosition > preBeatLeniency && beatPosition < 1 - postBeatLeniency)
		return false;

	//Set lastAttackBeat to the focus beat
	lastAttackBeat = musicManager->GetFocusedBeat();
	return true;
}

//Deals damage to hit actor
void UCombatSystemComponent::DealDamage(AActor*& actorHit, FS_DamageInfo& damageInfo)
{
	//Gets damage actor and execute TakeDamage
	if (!actorHit || !actorHit->Implements<UDamageableInterface>())
	{
		return;
	}

	FS_DamageInfo changedDamage = damageInfo;

	//changedDamage.damageAmount *= powerMult;
	if (musicMult > 0)
	{
		changedDamage.damageAmount *= musicMult;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString::SanitizeFloat(changedDamage.damageAmount));
		}
	}
	/*
	if (damageMults.Num() >= previousActions.Num())
	{
		//changedDamage.damageAmount *= damageMults[previousActions.Num() - 1];
	}
	if (previousActions.Num() && previousActions[previousActions.Num() - 1] == E_CombatActionType::HeavyAttack)
	{
		changedDamage.damageAmount *= 1.2f;
	}
	*/

	IDamageableInterface::Execute_TakeDamage(actorHit, changedDamage);
}

//Calculates multiplier for combos
float UCombatSystemComponent::MusicalMultiplier_Implementation()
{
	if (musicManager == nullptr)
		return 1;

	float multiplier = FMath::Abs(0.5f - musicManager->GetPositionOfBeat());

	if (multiplier >= 0.5 - critRange)
	{
		return critDamage;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString::SanitizeFloat(multiplier + 0.8f));
	}

	return multiplier + 0.8f;
}
#pragma endregion

#pragma region Downbeat
//Looks at down beat
void UCombatSystemComponent::DownBeat()
{
	//Check if music manager exists
	if (musicManager == nullptr)
		return;

	//Check if last attack equals the "focused" beat
	if (lastAttackBeat == musicManager->GetFocusedBeat() || previousDirection == E_CombatDirections::None)
		return;

	//If last attack is different, set a timer to reset inputs
	GetWorld()->GetTimerManager().SetTimer(resetAttackInfoTimer, this, &UCombatSystemComponent::ResetAttackInfo, resetTime);
}
#pragma endregion

#pragma region Old
/*
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

			timeBetweenInput = 0;
			canReadInput = false;
			world->GetTimerManager().SetTimer(readInputOnlyTimer, this, &UCombatSystemComponent::ResetReadInputsOnly, 0.1f, false);
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

	tickUp = false;

	//Inizialize some variables
	IAttacksInterface* attacker = Cast<IAttacksInterface>(owner);
	E_CombatActionType nextAction;
	combatQueue.Dequeue(nextAction);

	//Looks at the tail of queue and perform correlated actions. Then Pop
	switch (nextAction)
	{
		case E_CombatActionType::HeavyAttack:

			if (attacker)
			{

				//waitTime = attacker->Execute_HeavyAttack(Cast<UObject>(attacker), previousActions, false);
				if (previousActions.Num() == 0)
				{
					downSwing = true;
				}
				else
				{
					E_CombatActionType prev = previousActions[previousActions.Num() - 1];
					switch (prev)
					{
						case E_CombatActionType::HeavyAttack:
							if(previousActions.Num() == 2 && previousActions[0] == E_CombatActionType::HeavyAttack)
							{
								downSwing = true;
							}
							else
							{
								upSwing = true;
							}
							break;
						case E_CombatActionType::LightAttack:
							downSwing = true;
							break;
					}
				}
				previousActions.Add(E_CombatActionType::HeavyAttack);

			}
			break;
		case E_CombatActionType::LightAttack:
			if (attacker)
			{
				//waitTime = attacker->Execute_LightAttack(Cast<UObject>(attacker), previousActions);

				if (previousActions.Num() == 0)
				{
					leftSwing = true;
				}
				else
				{
					E_CombatActionType prev = previousActions[previousActions.Num() - 1];
					switch (prev)
					{
						case E_CombatActionType::HeavyAttack:
							rightSwing = true;
							break;
						case E_CombatActionType::LightAttack:
							if (previousActions.Num() == 1)
							{
								rightSwing = true;
							}
							else
							{
								leftSwing = true;
							}
							break;
					}
				}

				previousActions.Add(E_CombatActionType::LightAttack);

			}
			break;
	}

	//Ensures players can no longer attack
	canAttack = false;
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
*/
/*
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
	if (timeBetweenInput < maxTimeBetweenInput && tickUp)
	{
		timeBetweenInput += world->DeltaTimeSeconds;
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UCombatSystemComponent::QueueCountUp);
	}
	else if (tickUp)
	{
		ResetQueue(0.1f);
	}
}

//Resets the can attack variable to true
void UCombatSystemComponent::ResetCanAttack()
{
	//Lets player attack and recall read combat
	canAttack = true;
	//ReadCombatQueue();
}
*/

/*Emptys the queue and waits to let players attack
* @param waitTime The time the animation takes to play before player can attack again
*/
/*
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

void UCombatSystemComponent::AttackFinished()
{
	//Set all bool values for animation to 0
	upSwing = false;
	downSwing = false;
	leftSwing = false;
	rightSwing = false;

	if (previousActions.Num() >= MAX_COMBO_LENGTH)
	{
		//Prevents player from attack of inpit reading

		canReadInput = false;
		canAttack = false;

		/*
		world->GetTimerManager().ClearTimer(readInputOnlyTimer);
		world->GetTimerManager().ClearTimer(queueTimer);
		world->GetTimerManager().SetTimer(queueTimer, this, &UCombatSystemComponent::ResetReadInputs, .5f, false);
	}
	else
	{
		canAttack = true;
		//ReadCombatQueue();
		//timeBetweenInput = 0; 
		//tickUp = true;
		//QueueCountUp();
	}
	didCharge = false;
}
*/
#pragma endregion