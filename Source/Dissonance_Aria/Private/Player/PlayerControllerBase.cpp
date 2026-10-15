


#include "Player/PlayerControllerBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/PlayerCharacterBase.h"

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	//Assign player base
	playerBase = Cast<APlayerCharacterBase>(GetCharacter());

	//Add basic mapping
	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		subsystem->AddMappingContext(basicMapping, 2);

		if (playerBase && playerBase->GetUseFreeCamera())
		{
			subsystem->AddMappingContext(freeLookMapping, 1);
		}

		FInputModeGameOnly gameOnly;
		SetInputMode(gameOnly);
	}
}
