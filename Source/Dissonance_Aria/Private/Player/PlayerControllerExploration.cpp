


#include "Player/PlayerControllerExploration.h"

void APlayerControllerExploration::SetCurrentCamera(UCameraComponent* newCamera)
{
	SetViewTarget(newCamera->GetOwner());
}