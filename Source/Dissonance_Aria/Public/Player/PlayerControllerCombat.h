

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayerControllerBase.h"
#include "DamageSystem/DamageableInterface.h"
#include "PlayerControllerCombat.generated.h"

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

	virtual void AcknowledgePossession(APawn* InPawn) override;

#pragma region Setup and down
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SetUp")
	void SetUpUI();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DisconnentPlayerInput(int exp, int gold);
#pragma endregion
};