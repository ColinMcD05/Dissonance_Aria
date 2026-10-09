

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cameras/FixedCamera.h"
#include "CameraManager.generated.h"

class APlayerCharacterExploration;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraChange, AFixedCamera*, newCamera);

UCLASS()
class DISSONANCE_ARIA_API ACameraManager : public AActor
{
	GENERATED_BODY()

private:
	APlayerCharacterExploration* player;

	TArray<AFixedCamera*> cameras;

	AFixedCamera* currentCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ACameraManager();

	UFUNCTION()
	void SwitchCamera(AFixedCamera* newCamera);

	UFUNCTION()
	void CheckNearestCamera();

	AFixedCamera* FindNearestActor(UWorld* World, const FVector& FromLocation)

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnCameraChange OnCameraChange;
};
