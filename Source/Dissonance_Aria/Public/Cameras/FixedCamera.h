

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "FixedCamera.generated.h"


UCLASS()
class DISSONANCE_ARIA_API AFixedCamera : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* camera;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AFixedCamera();

	UCameraComponent* GetCamera() { return camera; }

	UFUNCTION(BlueprintCallable)
	void SetName();
};
