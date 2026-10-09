

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "FixedCamera.generated.h"



UCLASS()
class DISSONANCE_ARIA_API AFixedCamera : public AActor
{
	GENERATED_BODY()
	
private:
	UCameraComponent* camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AFixedCamera();

	UCameraComponent* GetCamera() { return camera; }
};
