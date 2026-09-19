


#include "DamageSystem/DamageSystemComponent.h"

// Sets default values for this component's properties
UDamageSystemComponent::UDamageSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	hatedBonus += 1;
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

bool UDamageSystemComponent::HandleIncomingDamage(FS_DamageInfo& damageInfo)
{
	if (isDead)
	{
		return false;
	}

	float damageTaken = damageInfo.damageAmount;

	if (damageInfo.genreAttack != EGenre::None)
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

	currentHealth = FMath::Clamp(currentHealth - damageTaken, 0.0f, maxHealth);
	return true;
}

void UDamageSystemComponent::HandleIncomingHeal(float healAmount, AActor* healer)
{
	if (isDead) 
	{ 
		return; 
	}

	currentHealth = FMath::Clamp(currentHealth + healAmount, 0.0f, maxHealth);
}