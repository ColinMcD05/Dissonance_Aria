

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "PlayerControllerBase.generated.h"

class APlayerCharacterBase;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* freeLookMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* combatMapping;
#pragma endregion

	APlayerCharacterBase* playerBase;

public:
	virtual void BeginPlay() override;

	//Pause the game
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PausedGame();

	//Set the current camera
	void SetCurrentCamera(UCameraComponent* newCamera);

#pragma region Setup and down Combat Character
	UFUNCTION()
	void CombatStart();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SetUp")
	void SwapUI(E_GameStateType newState);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DisconnentPlayerInput(int exp, int gold);
#pragma endregion
};
