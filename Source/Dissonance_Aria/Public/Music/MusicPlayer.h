

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"
#include "MusicPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBPMChanged, int, newBPM);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISSONANCE_ARIA_API UMusicPlayer : public UActorComponent
{
	GENERATED_BODY()

private:
	int bpm;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	UMusicPlayer();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region Timing
	int CheckTime();
#pragma endregion

#pragma region BPM
	void ChangeBPM(int newBPM);
#pragma endregion

#pragma region Delegates
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnBPMChanged OnBPMChanged;
#pragma endregion
};
