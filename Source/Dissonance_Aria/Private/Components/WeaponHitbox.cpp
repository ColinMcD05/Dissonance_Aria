


#include "Components/WeaponHitbox.h"

void UWeaponHitbox::BeginPlay()
{
	PrimaryComponentTick.bCanEverTick = false;

	attackingOwner = GetOwner();
	SetGenerateOverlapEvents(false);
}

void UWeaponHitbox::StartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	ICanAttackInterface* attacker = Cast<ICanAttackInterface>(attackingOwner);

	if (attacker)
	{
		attacker->Execute_HitActor(Cast<UObject>(attacker), otherActor);
	}
}