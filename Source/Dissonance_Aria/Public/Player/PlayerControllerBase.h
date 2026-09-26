

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "PlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class DISSONANCE_ARIA_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
protected:
#pragma region Inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* basicMapping;
#pragma endregion

public:
	virtual void BeginPlay() override;
};
