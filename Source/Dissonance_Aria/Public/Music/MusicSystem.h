

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"
#include "MusicSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBPMChanged, int, newBPM);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DISSONANCE_ARIA_API UMusicSystem : public UActorComponent
{
	GENERATED_BODY()

private:
	int bpm;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	UMusicSystem();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region BPM

#pragma endregion

#pragma region Delegates
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnBPMChanged OnBPMChanged;
#pragma endregion
};
