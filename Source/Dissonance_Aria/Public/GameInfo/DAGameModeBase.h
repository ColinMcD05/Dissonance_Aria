

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DAGameModeBase.generated.h"

class APlayerCharacterBase;
class APlayerCharacterExploration;
class APlayerControllerBase;

UENUM(BlueprintType)
enum class E_GameStateType : uint8
{
	Exploration UMETA(DisplayName = "Exploration"),
	Combat UMETA(DisplayName = "Combat"),
	Boss UMETA(DisplayName = "Boss")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameTypeChange, E_GameStateType, newState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueChanged, bool, dialogueActive);

/**
 * 
 */
UCLASS()
class DISSONANCE_ARIA_API ADAGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
private:
#pragma region LevelStreaming
	UPROPERTY(EditAnywhere)
	FString explorationLevel;

	FVector explorationLocation;

	UPROPERTY(EditAnywhere)
	FString combatLevel;

	UPROPERTY(EditAnywhere)
	FVector combatLocation;
#pragma endregion

#pragma region Character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//Player Controller
	APlayerControllerBase* controller;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//Explorer Character
	APlayerCharacterExploration* exploringCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//Combat Character
	APlayerCharacterBase* combatCharacter;
#pragma endregion

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Transitions")
	void TransitionToLevel(FName levelName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Transitions")
	void LevelOpened();

	UFUNCTION(BlueprintCallable, Category = "SwapLogic")
	void SwapGameState(E_GameStateType gameStateSwapped);

	void StartCombat();

	UFUNCTION(BlueprintCallable, Category = "SwapLogic")
	void PossessCombat();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SwapLogic")
	void LoadBoss();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SwapLogic")
	void LoadCombat();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SwapLogic")
	void LoadExploration();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Setup")
	void SetupCombatCharacter();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetupExplorerCharacter(APlayerControllerBase* newController, APlayerCharacterExploration* newExploringCharacter);

	UPROPERTY(BlueprintAssignable)
	FGameTypeChange OnGameTypeChange;

	UPROPERTY(BlueprintAssignable)
	FOnDialogueChanged OnDialogueChanged;
};
