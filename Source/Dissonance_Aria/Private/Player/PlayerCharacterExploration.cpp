


#include "Player/PlayerCharacterExploration.h"

APlayerCharacterExploration::APlayerCharacterExploration()
{

}

void APlayerCharacterExploration::Move_Implementation(const FInputActionValue& value)
{
	const FVector2D moveVector = value.Get<FVector2D>();

	const FRotator yaw(0, currentCamera->GetComponentRotation().Yaw, 0);

	FVector forward = FRotationMatrix(yaw).GetUnitAxis(EAxis::X);
	FVector right = FRotationMatrix(yaw).GetUnitAxis(EAxis::Y);

	AddMovementInput(right, moveVector.X);
	AddMovementInput(forward, moveVector.Y);
}