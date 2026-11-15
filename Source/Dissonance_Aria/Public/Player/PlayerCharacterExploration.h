

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

#pragma region Camera
	UCameraComponent* currentCamera;

	FVector forward;
	FVector right;
#pragma endregion

#pragma region Inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
	UInputAction* interactInput;
#pragma endregion

#pragma region Interaction
	AActor* interactingActor;
#pragma endregion

protected:
	virtual void BeginPlay() override;

public:

	virtual void Move_Implementation(const FInputActionValue& value) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region Movement
	//Set the new forward
	void SetNewForward();

	//Set the new rotation
	void SetRotation(FVector2D moveVector);
#pragma endregion

#pragma region Interaction
	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void Interact();

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void SetInteractingObject(AActor* newInteractingActor, bool leaving = false);
#pragma endregion

#pragma region Camera
	//Set current camera
	UFUNCTION(BlueprintCallable)
	void SetCurrentCamera(UCameraComponent* newCamera);

	UFUNCTION(BlueprintCallable)
	void SetBestCamera();

	//Get the nearest camera
	//AFixedCamera* FindNearestCamera();
#pragma endregion
};