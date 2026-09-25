

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "CombatSystem/HurtBoxInterface.h"
#include "WeaponHitbox.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Hitbox), meta = (BlueprintSpawnableComponent))
class DISSONANCE_ARIA_API UWeaponHitbox : public UCapsuleComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
	AActor* attackingOwner;

public:
	virtual void BeginPlay() override;

	void StartOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, 
		UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
};
