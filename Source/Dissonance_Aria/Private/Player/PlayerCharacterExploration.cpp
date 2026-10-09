


#include "Player/PlayerCharacterExploration.h"
#include "Kismet/GameplayStatics.h"
#include "Cameras/FixedCamera.h"
#include "EnhancedInputComponent.h"
#include "Player/PlayerControllerExploration.h"
#include "GameFramework/CharacterMovementComponent.h"

void APlayerCharacterExploration::BeginPlay()
{
	Super::BeginPlay();


	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	if (rotationSpeed < 100)
		rotationSpeed *= 100;

	if (AFixedCamera* fixedCamera = FindNearestActor(GetWorld(), GetActorLocation()))
	{
		SetCurrentCamera(fixedCamera->GetCamera());
		SetNewForward();
	}
}

void APlayerCharacterExploration::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		inputComponent->BindAction(GetMove(), ETriggerEvent::Completed, this, &APlayerCharacterExploration::SetNewForward);
	}
}

void APlayerCharacterExploration::Move_Implementation(const FInputActionValue& value)
{
	if (currentCamera)
	{
		const FVector2D moveVector = value.Get<FVector2D>();

		AddMovementInput(right, moveVector.X);
		AddMovementInput(forward, moveVector.Y);

		SetRotation(moveVector);
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

void APlayerCharacterExploration::SetNewForward()
{
	const FRotator yaw(0, currentCamera->GetComponentRotation().Yaw, 0);

	forward = FRotationMatrix(yaw).GetUnitAxis(EAxis::X);
	right = FRotationMatrix(yaw).GetUnitAxis(EAxis::Y);
}

void APlayerCharacterExploration::SetRotation(FVector2D moveVector)
{
	FVector direction = moveVector.X * forward + -right * moveVector.Y;

	if (!direction.IsNearlyZero())
	{
		FRotator rotation(0.f, direction.Rotation().Yaw, 0.f);

		FRotator newRotation = FMath::RInterpConstantTo(GetActorRotation(), FRotator(0, rotation.Yaw, 0), GetWorld()->GetDeltaSeconds(), rotationSpeed);

		SetActorRotation(newRotation);
	}
}

AFixedCamera* APlayerCharacterExploration::FindNearestActor(UWorld* World,const FVector& FromLocation)
{
	if (!World)
	{
		return nullptr;
	}

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(World, AFixedCamera::StaticClass(), actors);

	AFixedCamera* nearest = nullptr;
	float bestDistSquared = TNumericLimits<float>::Max();

	for (AActor* actor : actors)
	{
		if (!IsValid(actor))
		{
			continue;
		}
		AFixedCamera* camera = Cast< AFixedCamera>(actor);
		if (camera)
		{
			const float distSquared =
				FVector::DistSquared(FromLocation, camera->GetActorLocation());

			if (distSquared < bestDistSquared)
			{
				bestDistSquared = distSquared;
				nearest = camera;
			}
		}
	}

	return nearest; // nullptr if no valid matching actor
}