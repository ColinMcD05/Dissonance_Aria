


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
	IHurtBoxInterface* attacker = Cast<IHurtBoxInterface>(attackingOwner);

	if (attacker)
	{
		attacker->Execute_OverlappedActor(Cast<UObject>(attacker), otherActor);
	}
}