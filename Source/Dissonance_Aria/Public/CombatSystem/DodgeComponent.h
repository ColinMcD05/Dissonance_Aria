

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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dodge", meta = (AllowPrivateAccess = "true"))
	float timeToDodge;

	bool dodging = false;

	E_Directions currentDirection = E_Directions::None;

	float distance = 0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	// Sets default values for this component's properties
	UDodgeComponent();

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	E_Directions CalculateDodgeDirection();

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void StartDodge(E_Directions direction, float timing);

	void Dodge();

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void EndDodge();

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void ResetDodge();

#pragma region Delegates
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDodge OnDodge;
#pragma endregion
};
