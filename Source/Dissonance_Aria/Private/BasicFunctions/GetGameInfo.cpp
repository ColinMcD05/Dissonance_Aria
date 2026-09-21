


#include "BasicFunctions/GetGameInfo.h"
#include "GameInfo/DissonanceAriaGameInstance.h"

namespace GameInfoUtilities
{
	UDissonanceAriaGameInstance* GetDAGameInstance(const AActor* actor)
	{
		return Cast<UDissonanceAriaGameInstance>(actor->GetGameInstance());
	}
}
