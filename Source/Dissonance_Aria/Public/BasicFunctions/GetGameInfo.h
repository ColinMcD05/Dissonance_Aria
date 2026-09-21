

#pragma once


#include "CoreMinimal.h"

class AActor;
class UDissonanceAriaGameInstance;
/**
 * 
 */
namespace GameInfoUtilities
{
	UDissonanceAriaGameInstance* GetDAGameInstance(const AActor* player);
}
