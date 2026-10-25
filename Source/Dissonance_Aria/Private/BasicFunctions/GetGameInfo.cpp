


#include "BasicFunctions/GetGameInfo.h"
#include "GameInfo/DAGameInstance.h"
#include "GameInfo/DAGameStateCombat.h"

namespace GameInfoUtilities
{
	UDAGameInstance* GetDAGameInstance(AActor* actor)
	{
		return Cast<UDAGameInstance>(actor->GetGameInstance());
	}

	void PrintToScreen(FString string)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, string);
		}
	}
}
