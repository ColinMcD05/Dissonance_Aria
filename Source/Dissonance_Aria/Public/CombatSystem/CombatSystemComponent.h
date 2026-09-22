

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Components/ActorComponent.h"
#include "DamageSystem/DamageInfo.h"
#include "Input/InputActions/CombatActionBase.h"
#include "CombatSystemComponent.generated.h"

class APlayerCharacterCombat;

DECLARE_

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISSONANCE_ARIA_API UCombatSystemComponent : public UActorComponent
{
	GENERATED_BODY()

private:
#pragma region Input
	TQueue<E_CombatActionType> combatQueue;
	int queueCount;
#pragma endregion
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UCombatSystemComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddToCombatQueue(E_CombatActionType& action);

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void DealDamage(AActor*& enemyHit, FS_DamageInfo& damageInfo);

	void ReadCombat();
};
