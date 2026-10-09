


#include "Player/PlayerCharacterExploration.h"
#include "Kismet/GameplayStatics.h"
#include "Cameras/FixedCamera.h"
#include "Player/PlayerControllerExploration.h"
void APlayerCharacterExploration::BeginPlay()
{
	Super::BeginPlay();
	AActor* actor = UGameplayStatics::GetActorOfClass(this, AFixedCamera::StaticClass());
	if (AFixedCamera* fixedCamera = Cast<AFixedCamera>(actor))
	{
		SetCurrentCamera(fixedCamera->GetCamera());
	}
}

void APlayerCharacterExploration::Move_Implementation(const FInputActionValue& value)
{
	if (currentCamera)
	{
		const FVector2D moveVector = value.Get<FVector2D>();

		const FRotator yaw(0, currentCamera->GetComponentRotation().Yaw, 0);

		FVector forward = FRotationMatrix(yaw).GetUnitAxis(EAxis::X);
		FVector right = FRotationMatrix(yaw).GetUnitAxis(EAxis::Y);

		AddMovementInput(right, moveVector.X);
		AddMovementInput(forward, moveVector.Y);
	}
}

void APlayerCharacterExploration::SetCurrentCamera(UCameraComponent* newCamera)
{
	currentCamera = newCamera;
	if (APlayerControllerExploration* controller = Cast<APlayerControllerExploration>(GetController()))
	{
		controller->SetCurrentCamera(newCamera);
	}
}