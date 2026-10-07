


#include "Player/PlayerControllerCombat.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/PlayerCharacterCombat.h"

void APlayerControllerCombat::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		subsystem->AddMappingContext(combatMapping, 1);
	}

	if (ADAGameStateCombat* gameState = GameInfoUtilities::GetDAGameState<ADAGameStateCombat>(this))
	{
		gameState->OnCombatEnd.AddDynamic(this, &APlayerControllerCombat::DisconnentPlayerInput);
	}
}

void  APlayerControllerCombat::AcknowledgePossession(APawn* InPawn)
{
	Super::AcknowledgePossession(InPawn);

	if (APlayerCharacterCombat* playerCharacter = Cast<APlayerCharacterCombat>(InPawn))
	{
		playerCharacter->OnPlayerSetUpDone.AddDynamic(this, &APlayerControllerCombat::SetUpUI);
	}
}

void APlayerControllerCombat::DisconnentPlayerInput(int exp)
{
	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		subsystem->ClearAllMappings();
	}
}