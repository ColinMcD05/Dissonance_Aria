


#include "Player/PlayerCharacterExploration.h"
#include "Kismet/GameplayStatics.h"
#include "Cameras/FixedCamera.h"
#include "EnhancedInputComponent.h"
#include "Player/PlayerControllerExploration.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BasicFunctions/GetGameInfo.h"
#include "GameInfo/DAGameInstance.h"

void APlayerCharacterExploration::BeginPlay()
{
	Super::BeginPlay();


	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	if (rotationSpeed < 100)
		rotationSpeed *= 100;

	TArray<AActor*> cameras;
	UGameplayStatics::GetAllActorsOfClass(this, AFixedCamera::StaticClass(), cameras);

	UDAGameInstance* instance = GameInfoUtilities::GetDAGameInstance(this);
	if (!instance || instance->GetLastCamera().IsEmpty())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Bad"));
		}
		float distance;

		SetCurrentCamera(Cast<AFixedCamera>(UGameplayStatics::FindNearestActor(GetActorLocation(), cameras, distance))->GetCamera());
	}
	else
	{
		for (AActor* actor : cameras)
		{
			if (actor->GetName() == instance->GetLastCamera())
			{
				SetCurrentCamera(Cast<AFixedCamera>(actor)->GetCamera());
				break;
			}
		}
	}

	SetNewForward();
}

void APlayerCharacterExploration::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		inputComponent->BindAction(GetMove(), ETriggerEvent::Completed, this, &APlayerCharacterExploration::SetNewForward);
	}
}

#pragma region Movement
//Move player
void APlayerCharacterExploration::Move_Implementation(const FInputActionValue& value)
{
	if (currentCamera)
	{
		//Move based on camera forward
		const FVector2D moveVector = value.Get<FVector2D>();

		AddMovementInput(right, moveVector.X);
		AddMovementInput(forward, moveVector.Y);

		SetRotation(moveVector);
	}
}

//Set the new vector considered forward
void APlayerCharacterExploration::SetNewForward()
{
	if (currentCamera)
	{
		const FRotator yaw(0, currentCamera->GetComponentRotation().Yaw, 0);

		forward = FRotationMatrix(yaw).GetUnitAxis(EAxis::X);
		right = FRotationMatrix(yaw).GetUnitAxis(EAxis::Y);
	}
}

//Set player rotation
void APlayerCharacterExploration::SetRotation(FVector2D moveVector)
{
	if (currentCamera)
	{
		//Get direction
		FVector direction = moveVector.X * forward + -right * moveVector.Y;

		//Rotate player
		if (!direction.IsNearlyZero())
		{
			FRotator rotation(0.f, direction.Rotation().Yaw, 0.f);

			FRotator newRotation = FMath::RInterpConstantTo(GetActorRotation(), FRotator(0, rotation.Yaw, 0), GetWorld()->GetDeltaSeconds(), rotationSpeed);

			SetActorRotation(newRotation);
		}
	}
}
#pragma endregion

#pragma region Camera
void APlayerCharacterExploration::SetCurrentCamera(UCameraComponent* newCamera)
{
	//Sets the new camera
	currentCamera = newCamera;
	if (APlayerControllerExploration* controller = Cast<APlayerControllerExploration>(GetController()))
	{
		controller->SetCurrentCamera(newCamera);
	}
}
#pragma endregion