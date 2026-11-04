


#include "GameInfo/DAGameModeBase.h"
#include "GameInfo/DAGameStateCombat.h"
#include "BasicFunctions/GetGameInfo.h"
#include "Player/PlayerCharacterBase.h"
#include "Player/PlayerControllerBase.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"

void ADAGameModeBase::TransitionToLevel_Implementation(FName levelName)
{
	UGameplayStatics::OpenLevel(GetWorld(), levelName);
}

void ADAGameModeBase::LevelOpened_Implementation()
{
}

void ADAGameModeBase::SwapGameState(E_GameStateType gameStateSwapped)
{
	switch (gameStateSwapped)
	{
		case E_GameStateType::Combat:
			LoadCombat();
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ADAGameModeBase::StartCombat);
			break;
		case E_GameStateType::Exploration:
			LoadExploration();
			break;
	}
	OnGameTypeChange.Broadcast(gameStateSwapped);
}

void ADAGameModeBase::StartCombat()
{
	if (ADAGameStateCombat* state = Cast<ADAGameStateCombat>(GameState))
	{
		state->OnCombatStart.Broadcast();
	}
}

void ADAGameModeBase::SetupExplorerCharacter(APlayerControllerBase* newController, APlayerCharacterBase* newExploringCharacter)
{ 
	controller = newController; 
	exploringCharacter = newExploringCharacter; 
}

#pragma region Swapping
void ADAGameModeBase::LoadCombat_Implementation()
{
	if (!IsValid(combatCharacter))
	{
		GameInfoUtilities::PrintToScreen("Not there.");
		return;
	}

	GameInfoUtilities::PrintToScreen("Is there.");
	controller->Possess(combatCharacter);
}

void ADAGameModeBase::LoadExploration_Implementation()
{
	controller->Possess(exploringCharacter);
}
#pragma endregion