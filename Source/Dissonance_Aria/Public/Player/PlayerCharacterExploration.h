

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayerCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "PlayerCharacterExploration.generated.h"

class AFixedCamera;

/**
 * 
 */
UCLASS()
class DISSONANCE_ARIA_API APlayerCharacterExploration : public APlayerCharacterBase
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float rotationSpeed = 6;

	FVector forward;
	FVector right;
#pragma region Camera
	UCameraComponent* currentCamera;
#pragma endregion

protected:
	virtual void BeginPlay() override;

public:

	virtual void Move_Implementation(const FInputActionValue& value) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetCurrentCamera(UCameraComponent* newCamera);

	void SetNewForward();

	void SetRotation(FVector2D moveVector);

	AFixedCamera* FindNearestActor(UWorld* World, const FVector& FromLocation);
};