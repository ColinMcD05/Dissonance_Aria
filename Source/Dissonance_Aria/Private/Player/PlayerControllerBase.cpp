


#include "Player/PlayerControllerBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameInfo/DAGameStateCombat.h"
#include "GameInfo/DAGameModeBase.h"
#include "Player/PlayerCharacterBase.h"
#include "BasicFunctions/GetGameInfo.h"

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	//Assign player base
	playerBase = Cast<APlayerCharacterBase>(GetCharacter());

	if (ADAGameStateCombat* gameState = GameInfoUtilities::GetDAGameState<ADAGameStateCombat>(this))
	{
		gameState->OnCombatStart.AddDynamic(this, &APlayerControllerBase::CombatStart);
		gameState->OnCombatEnd.AddDynamic(this, &APlayerControllerBase::DisconnentPlayerInput);
	}

	if (ADAGameModeBase* gameMode = Cast<ADAGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		gameMode->OnGameTypeChange.AddDynamic(this, &APlayerControllerBase::SwapUI);
	}

	//Add basic mapping
	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		subsystem->AddMappingContext(basicMapping, 1);

		if (playerBase && playerBase->GetUseFreeCamera())
		{
			subsystem->AddMappingContext(freeLookMapping, 0);
		}

		FInputModeGameOnly gameOnly;
		SetInputMode(gameOnly);
	}
}

void APlayerControllerBase::SetCurrentCamera(UCameraComponent* newCamera)
{
	SetViewTarget(newCamera->GetOwner());
}

#pragma region Setup and down
void APlayerControllerBase::CombatStart()
{
	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		subsystem->AddMappingContext(combatMapping, 2);
	}
}

//Clears all mappings
void APlayerControllerBase::DisconnentPlayerInput(int exp, int gold)
{
	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		subsystem->RemoveMappingContext(combatMapping);
	}
	UnPossess();
}
#pragma endregion