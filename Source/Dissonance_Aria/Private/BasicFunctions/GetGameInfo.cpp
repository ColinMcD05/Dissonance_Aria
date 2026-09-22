


#include "BasicFunctions/GetGameInfo.h"
#include "GameInfo/DAGameInstance.h"

namespace GameInfoUtilities
{
	UDAGameInstance* GetDAGameInstance(const AActor* actor)
	{
		return Cast<UDAGameInstance>(actor->GetGameInstance());
	}
}
