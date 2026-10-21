


#include "CombatSystem/DodgeComponent.h"
#include "KismetAnimationLibrary.h"

// Sets default values for this component's properties
UDodgeComponent::UDodgeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDodgeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

E_Directions UDodgeComponent::CalculateDodgeDirection()
{
	AActor* owner = GetOwner();
	if (!IsValid(owner))
		return E_Directions::Forward;

	float direction = UKismetAnimationLibrary::CalculateDirection(owner->GetVelocity(), owner->GetActorRotation());

	if (direction == 0 || (direction <= -110 && direction <= 60))
		return E_Directions::Backward;

	if (direction > -110 && direction < 0)
		return E_Directions::Left;

	if (direction < 180 && direction > 60)
		return E_Directions::Right;

	else return E_Directions::Backward;
}

void UDodgeComponent::StartDodge(E_Directions direction, float distance)
{
	if (dodging)
		return;

	if (direction == E_Directions::None)
		direction = CalculateDodgeDirection();

	OnDodge.Broadcast(direction);

	dodging = true;

	this->currentDirection = direction;
	this->distance = distance;

	Dodge();
}

void UDodgeComponent::Dodge()
{


	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UDodgeComponent::Dodge);
}

void UDodgeComponent::EndDodge()
{
	OnDodge.Broadcast(E_Directions::None);
	dodging = false;
}

void UDodgeComponent::ResetDodge()
{
	OnDodge.Broadcast(E_Directions::None);
}