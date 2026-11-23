


#include "Player/PlayerCharacterExploration.h"
#include "Player/PlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Cameras/FixedCamera.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BasicFunctions/GetGameInfo.h"
#include "GameInfo/DAGameInstance.h"
#include "FMODBlueprintStatics.h"
#include "Level/FogMinigame.h"
#include "Interactable/InteractableInterface.h"

void APlayerCharacterExploration::BeginPlay()
{
	Super::BeginPlay();

	UDAGameInstance* instance = GameInfoUtilities::GetDAGameInstance(this);

	if (IsValid(instance))
	{
		inventory = instance->GetInventory();
	}

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	if (rotationSpeed < 100)
		rotationSpeed *= 100;

	SetBestCamera();

	SetNewForward();
}

void APlayerCharacterExploration::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		inputComponent->BindAction(GetMove(), ETriggerEvent::Completed, this, &APlayerCharacterExploration::SetNewForward);
		inputComponent->BindAction(interactInput, ETriggerEvent::Started, this, &APlayerCharacterExploration::Interact);

		inputComponent->BindAction(startSongInput, ETriggerEvent::Started, this, &APlayerCharacterExploration::MinigameStarted);
		inputComponent->BindAction(startSongInput, ETriggerEvent::Completed, this, &APlayerCharacterExploration::MinigameStopped);
		inputComponent->BindAction(performSongInput, ETriggerEvent::Started, this, &APlayerCharacterExploration::PerformSong);

		APlayerControllerBase* controller = Cast<APlayerControllerBase>(Controller);

		if (IsValid(controller))
		{
			inputComponent->BindAction(startSongInput, ETriggerEvent::Started, controller, &APlayerControllerBase::StartMinigame);
			inputComponent->BindAction(startSongInput, ETriggerEvent::Completed, controller, &APlayerControllerBase::StopMinigame);
		}
	}
}

#pragma region Movement
//Move player
void APlayerCharacterExploration::Move_Implementation(const FInputActionValue& value)
{
	if (!canReadInput)
		return;

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

#pragma region Interact
void APlayerCharacterExploration::Interact_Implementation()
{
	if (!IsValid(interactingActor) || !interactingActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		return;
	}

	IInteractableInterface::Execute_Interact(interactingActor, this);
}

void APlayerCharacterExploration::SetInteractingObject(AActor* newInteractingActor, bool leaving)
{
	if (!IsValid(newInteractingActor) || !newInteractingActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		interactingActor = nullptr;

		GameInfoUtilities::PrintToScreen("None");

		return;
	}

	if (newInteractingActor == interactingActor && leaving)
	{
		interactingActor = nullptr;
		GameInfoUtilities::PrintToScreen("None");
	}
	else if(!leaving)
	{
		interactingActor = newInteractingActor;

		GameInfoUtilities::PrintToScreen(newInteractingActor->GetName());
	}
}
#pragma endregion

#pragma region Camera
void APlayerCharacterExploration::SetBestCamera()
{

	TArray<AActor*> cameras;
	UGameplayStatics::GetAllActorsOfClass(this, AFixedCamera::StaticClass(), cameras);

	UDAGameInstance* instance = GameInfoUtilities::GetDAGameInstance(this);
	if (!instance || instance->GetLastCamera().IsEmpty())
	{
		float distance;

		AFixedCamera* actor = Cast<AFixedCamera>(UGameplayStatics::FindNearestActor(GetActorLocation(), cameras, distance));

		SetCurrentCamera(actor->GetCamera());
		instance->SetLastCamera(actor->GetName());
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
}

void APlayerCharacterExploration::SetCurrentCamera(UCameraComponent* newCamera)
{
	//Sets the new camera
	currentCamera = newCamera;
	if (APlayerControllerBase* controller = Cast<APlayerControllerBase>(GetController()))
	{
		controller->SetCurrentCamera(newCamera);
	}
}
#pragma endregion

#pragma region Music Minigame
void APlayerCharacterExploration::MinigameStarted()
{
	notes.Empty();
	performingMinigame = true;
}

void APlayerCharacterExploration::MinigameStopped()
{
	performingMinigame = false;

}

void APlayerCharacterExploration::PerformSong(const FInputActionValue& value)
{
	if(!performingMinigame)
		return;

	const int input = value.Get<float>();

	UPDA_Notes* inputNotes = GetNoteFromInput(input);

	if (IsValid(inputNotes) && IsValid(inputNotes->sound))
		UFMODBlueprintStatics::PlayEvent2D(this, inputNotes->sound, true);

	notes.Add(inputNotes);
	GameInfoUtilities::PrintToScreen(inputNotes->note);
	GameInfoUtilities::PrintToScreen(FString::FromInt(notes.Num()));


	if (notes.Num() >= MAX_NOTES)
	{
		if (IsValid(inventory))
		{
			UPDA_SongInfo* playedSong = inventory->CompareSong(notes);

			if (IsValid(playedSong))
			{
				GameInfoUtilities::PrintToScreen(playedSong->name);
				
				if (IsValid(currentMinigame))
				{
					UPDA_SongInfo* minigameSong = currentMinigame->GetAttachedSong();

					if (IsValid(minigameSong))
					{
						if (minigameSong->name.Compare(playedSong->name) == 0)
						{
							currentMinigame->RemoveFog();
						}
					}
				}

			}
			else
				GameInfoUtilities::PrintToScreen("None");
		}


		MinigameStopped();


		APlayerControllerBase* controller = Cast<APlayerControllerBase>(Controller);
		controller->StopMinigame();
	}
}
#pragma endregion