

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayerCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "PlayerCharacterExploration.generated.h"

/**
 * 
 */
UCLASS()
class DISSONANCE_ARIA_API APlayerCharacterExploration : public APlayerCharacterBase
{
	GENERATED_BODY()
	
private:

#pragma region Camera
	UCameraComponent* currentCamera;


#pragma endregion

public:
	APlayerCharacterExploration();

	virtual void Move_Implementation(const FInputActionValue& value) override;
};
