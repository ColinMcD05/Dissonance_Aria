

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/Directions.h"
#include "Components/ActorComponent.h"
#include "DodgeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDodge, E_Directions, currentDirection);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISSONANCE_ARIA_API UDodgeComponent : public UActorComponent
{
	GENERATED_BODY()

private:

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	// Sets default values for this component's properties
	UDodgeComponent();

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	E_Directions CalculateDodgeDirection();

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void Dodge(E_Directions direction);

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void ResetDodge();

#pragma region Delegates
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDodge OnDodge;
#pragma endregion
};
