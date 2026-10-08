

#pragma once


#include "CoreMinimal.h"

class AActor;
class UDAGameInstance;
class AGameStateBase;
class AGameModeBase;
/**
 * 
 */
namespace GameInfoUtilities
{
	//Returns the game instance
	UFUNCTION(BlueprintCallable)
	UDAGameInstance* GetDAGameInstance(AActor* actor);

	//Sets up to return and accept only game states
	template <typename T>
	concept GameStateType = std::derived_from<T, AGameStateBase>;

	//Returns the current game state
	UFUNCTION(BlueprintCallable)
	template<GameStateType T>
	T* GetDAGameState(AActor * actor)
	{
		return Cast<T>(actor->GetWorld()->GetGameState());
	}

	template <typename T>
	concept GameModeType = std::derived_from<T, AGameModeBase>;

	//Returns the current game state
	UFUNCTION(BlueprintCallable)
	template<GameModeType T>
	T* GetDAGameMode(AActor* actor)
	{
		return Cast<T>(actor->GetWorld()->GetAuthGameMode());
	}
}
