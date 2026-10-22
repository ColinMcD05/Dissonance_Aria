

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/Directions.h"
#include "Components/ActorComponent.h"
#include "DodgeComponent.generated.h"

class APlayerCharacterCombat;
class AMusicManagerCombat;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDodge, E_Directions, currentDirection);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISSONANCE_ARIA_API UDodgeComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dodge", meta = (AllowPrivateAccess = "true"))
	double timeToDodge;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dodge", meta = (AllowPrivateAccess = "true", ClampMin = "100"))
	float baseDistance = 200;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dodge", meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "0.15"))
	float perfectDodgeRange = 0.15f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dodge", meta = (AllowPrivateAccess = "true"))
	float perfectDodgeMult = 2;

	double speed;

	bool dodging = false;

	FVector endPoint;

	APlayerCharacterCombat* player;

	UPROPERTY()
	TObjectPtr<AMusicManagerCombat> musicManager;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	// Sets default values for this component's properties
	UDodgeComponent();

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	E_Directions CalculateDodgeDirection();

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void StartDodge(E_Directions direction = E_Directions::None);

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
