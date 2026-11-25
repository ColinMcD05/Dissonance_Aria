


#include "Player/PlayerCharacterExploration.h"
#include "Player/PlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Cameras/FixedCamera.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BasicFunctions/GetGameInfo.h"
#include "GameInfo/DAGameInstance.h"
#include "GameInfo/DAGameModeBase.h"
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

		controller = Cast<APlayerControllerBase>(Controller);
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
	if (!canReadInput && !inDialogue)
		return;

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
	if (IsValid(controller))
	{
		controller->SetCurrentCamera(newCamera);
	}
}
#pragma endregion

#pragma region Music Minigame
void APlayerCharacterExploration::MinigameStarted()
{
	if (!canReadInput)
		return;

	notes.Empty();
	performingMinigame = true;
	canReadInput = false;
	controller->StartMinigame();
}

void APlayerCharacterExploration::MinigameStopped()
{
	if (!performingMinigame)
		return;

	performingMinigame = false;
	canReadInput = true;
	controller->StopMinigame();
}

void APlayerCharacterExploration::PerformSong(const FInputActionValue& value)
{
	if(!performingMinigame)
		return;

	const int input = value.Get<float>();

	UPDA_Notes* inputNotes = GetNoteFromInput(input);

	if (IsValid(inputNotes) && IsValid(inputNotes->sound))
		UFMODBlueprintStatics::PlayEvent2D(this, inputNotes->sound, true);

	if (IsValid(controller))
	{
		controller->SendNoteInput(inputNotes);
	}

	notes.Add(inputNotes);

	if (notes.Num() >= MAX_NOTES)
	{
		performingMinigame = false;

		float waitTime = 0.5f;

		if (IsValid(inventory))
		{
			UPDA_SongInfo* playedSong = inventory->CompareSong(notes);

			if (IsValid(playedSong))
			{
				if (IsValid(controller))
				{
					controller->SendPlayedSong(playedSong);
				}
				
				if (IsValid(playedSong->song))
				{
					currentSong = UFMODBlueprintStatics::PlayEvent2D(this, playedSong->song, true);
					waitTime = playedSong->length + 1;
				}

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
		}

		GetWorld()->GetTimerManager().SetTimer(minigameDoneTimer, this, &APlayerCharacterExploration::MinigameDone, waitTime);
	}
}

void APlayerCharacterExploration::MinigameDone()
{
	UFMODBlueprintStatics::EventInstanceStop(currentSong);

	performingMinigame = true;
	MinigameStopped();
}
#pragma endregion

#pragma region Dialogue
void APlayerCharacterExploration::SetInDialogue(bool newInDialogue)
{
	inDialogue = newInDialogue;
	canReadInput = !newInDialogue;

	GameInfoUtilities::GetDAGameMode<ADAGameModeBase>(this)->OnDialogueChanged.Broadcast(newInDialogue);
}
#pragma endregion