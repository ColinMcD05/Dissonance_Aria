

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DAGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DISSONANCE_ARIA_API ADAGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Transitions")
	void TransitionToLevel(FName levelName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Transitions")
	void OpenLevel();
};
