

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayerControllerBase.h"
#include "PlayerControllerCombat.generated.h"

/**
 * 
 */
UCLASS()
class DISSONANCE_ARIA_API APlayerControllerCombat : public APlayerControllerBase
{
	GENERATED_BODY()

protected:
#pragma region Inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* combatMapping;
#pragma endregion

public:
	virtual void BeginPlay() override;
};
