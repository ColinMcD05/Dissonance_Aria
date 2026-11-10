


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
			exploringCharacter->canReadInput = false;
			LoadCombat();
			break;
		case E_GameStateType::Exploration:
			exploringCharacter->canReadInput = true;
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

void ADAGameModeBase::PossessCombat()
{
	controller->Possess(combatCharacter);
}

#pragma region Swapping
void ADAGameModeBase::LoadCombat_Implementation()
{
	if (!IsValid(combatCharacter))
	{
		//GameInfoUtilities::PrintToScreen("Not there.");
		return;
	}

	//GameInfoUtilities::PrintToScreen("Is there.");
}

void ADAGameModeBase::LoadExploration_Implementation()
{
	controller->Possess(exploringCharacter);
}
#pragma endregion