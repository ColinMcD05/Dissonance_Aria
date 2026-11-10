


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
		gameState->OnCombatStart.AddUniqueDynamic(this, &APlayerControllerBase::CombatStart);
		gameState->OnCombatEnd.AddUniqueDynamic(this, &APlayerControllerBase::DisconnentPlayerInput);
	}

	if (ADAGameModeBase* gameMode = Cast<ADAGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		gameMode->OnGameTypeChange.AddUniqueDynamic(this, &APlayerControllerBase::SwapUI);
	}

	//Add basic mapping
	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		subsystem->AddMappingContext(basicMapping, 1);
		subsystem->AddMappingContext(explorationMapping, 1);

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
	if(IsValid(newCamera))
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

	GetWorld()->GetTimerManager().SetTimer(canGoBackTimer, this, &APlayerControllerBase::CanGoBackToLevel, 0.5f);
}

void APlayerControllerBase::CanGoBackToLevel()
{
	InputComponent->BindKey(EKeys::AnyKey, IE_Pressed, this, &APlayerControllerBase::BackToLevel);

	canGoBack = true;
}

void APlayerControllerBase::PlayerDeath_Implementation()
{
	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		subsystem->RemoveMappingContext(combatMapping);
	}
	UnPossess();
}

void APlayerControllerBase::BackToLevel()
{
	if (canGoBack)
	{
		canGoBack = false;
		InputComponent->RemoveAxisKeyBinding(EKeys::AnyKey);

		GameInfoUtilities::GetDAGameMode<ADAGameModeBase>(this)->SwapGameState(E_GameStateType::Exploration);
	}
}
#pragma endregion