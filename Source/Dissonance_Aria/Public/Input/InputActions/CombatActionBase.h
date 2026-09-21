

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "CombatActionBase.generated.h"


UENUM(BlueprintType)
enum class E_CombatActionType : uint8
{
	LightAttack UMETA(DisplayName = "LightAttack"),
	HeavyAttack UMETA(DisplayName = "HeavyAttack"),
};

UCLASS()
class DISSONANCE_ARIA_API UCombatActionBase : public UInputAction
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	E_CombatActionType combatActionType;
};
