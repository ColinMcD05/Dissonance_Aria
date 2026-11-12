


#include "DamageSystem/DamageSystemComponent.h"
#include "BasicFunctions/GetGameInfo.h"

// Sets default values for this component's properties
UDamageSystemComponent::UDamageSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	hatedBonus += 1;

	currentHealth = maxHealth;
}


// Called when the game starts
void UDamageSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDamageSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

#pragma region Health
bool UDamageSystemComponent::HandleIncomingDamage(FS_DamageInfo damageInfo, int& damageAmount)
{
	if (isDead || invincible)
	{
		return false;
	}

	float damageTaken = damageInfo.damageAmount;

	if (damageInfo.genreAttack != E_Genre::None)
	{
		if (damageInfo.genreAttack == favoriteGenre)
		{
			damageTaken -= damageTaken * favortieResistance;
		}

		if (damageInfo.genreAttack == hatedGenre)
		{
			damageTaken *= hatedBonus;
		}
	}

	damageAmount = damageTaken;

	currentHealth = FMath::Clamp(currentHealth - damageTaken, 0.0f, maxHealth);

	if (currentHealth <= 0)
	{
		isDead = true;
	}

	OnTakeDamage.Broadcast(currentHealth);
	StartIFrames();
	
	OnReactionChanged.Broadcast(damageInfo.damageResponse);

	return true;
}

void UDamageSystemComponent::HandleIncomingHeal(float healPercent, AActor* healer)
{
	if (isDead) 
	{ 
		return; 
	}

	float newHealth = maxHealth * healPercent;

	currentHealth = FMath::Clamp(currentHealth + newHealth, 0.0f, maxHealth);

	OnHealthChange.Broadcast(maxHealth, currentHealth);
}

void UDamageSystemComponent::SetMaxHealthAndCurrent(int newMaxHealth)
{
	float percentage = currentHealth/maxHealth;

	maxHealth = newMaxHealth;
	currentHealth = FMath::Clamp(maxHealth * percentage, 0, maxHealth);

	OnHealthChange.Broadcast(maxHealth, currentHealth);
}

void UDamageSystemComponent::SetInitialHealth(int newHealth)
{
	maxHealth = newHealth;
	currentHealth = newHealth;

	OnHealthChange.Broadcast(maxHealth, currentHealth);

	isDead = false;
	invincible = false;
}

void UDamageSystemComponent::IncreaseMaxHealth(int increasedAmount)
{
	SetMaxHealthAndCurrent(maxHealth + increasedAmount);
}
#pragma endregion

#pragma region Invincibility
void UDamageSystemComponent::StartIFrames()
{
	invincible = true;
	GetWorld()->GetTimerManager().SetTimer(iFrameTimer, this, &UDamageSystemComponent::ResetIFrames, iFrameTime);
}

void UDamageSystemComponent::ResetIFrames()
{
	GameInfoUtilities::PrintToScreen("Reset");
	invincible = false;
}
#pragma endregion