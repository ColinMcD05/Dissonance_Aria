


#include "BasicFunctions/GetGameInfo.h"
#include "GameInfo/DissonanceAriaGameInstance.h"

namespace GameInfoUtilities
{
	UDissonanceAriaGameInstance* GetDAGameInstance(const AActor* player)
	{
		return Cast<UDissonanceAriaGameInstance>(player->GetGameInstance());
	}
}
