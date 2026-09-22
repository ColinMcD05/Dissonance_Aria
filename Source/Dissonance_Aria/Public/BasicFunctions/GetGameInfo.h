

#pragma once


#include "CoreMinimal.h"

class AActor;
class UDAGameInstance;
/**
 * 
 */
namespace GameInfoUtilities
{
	UDAGameInstance* GetDAGameInstance(const AActor* actor);
}
