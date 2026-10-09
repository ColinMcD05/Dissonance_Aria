

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayerControllerBase.h"
#include "Camera/CameraComponent.h"
#include "PlayerControllerExploration.generated.h"

/**
 * 
 */
UCLASS()
class DISSONANCE_ARIA_API APlayerControllerExploration : public APlayerControllerBase
{
	GENERATED_BODY()
public:
	void SetCurrentCamera(UCameraComponent* newCamera);
};
