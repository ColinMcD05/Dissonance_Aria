


#include "BasicFunctions/GetGameInfo.h"
#include "GameInfo/DAGameInstance.h"

namespace GameInfoUtilities
{
	UDAGameInstance* GetDAGameInstance(AActor* actor)
	{
		return Cast<UDAGameInstance>(actor->GetGameInstance());
	}
}
