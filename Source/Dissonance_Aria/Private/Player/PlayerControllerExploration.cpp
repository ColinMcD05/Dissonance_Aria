


#include "Player/PlayerControllerExploration.h"

//Sets the current camera
void APlayerControllerExploration::SetCurrentCamera(UCameraComponent* newCamera)
{
	SetViewTarget(newCamera->GetOwner());
}