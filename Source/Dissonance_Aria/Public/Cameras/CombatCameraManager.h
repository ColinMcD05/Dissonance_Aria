

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Actor.h"
#include "CombatCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class DISSONANCE_ARIA_API ACombatCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
private:
	AActor* focusedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float distanceFromPlayer;

public:
	void GetFocusPosition();

	//Setters
	void SetFocusedActor(AActor* newFocus) { focusedActor = newFocus; }
};
