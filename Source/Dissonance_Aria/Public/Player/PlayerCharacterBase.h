

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Player/PlayerWalkSpeeds.h"
#include "PlayerCharacterBase.generated.h"

UCLASS()
class DISSONANCE_ARIA_API APlayerCharacterBase : public ACharacter
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
	UInputAction* move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
	UInputAction* sprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
	UInputAction* jumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
	UInputAction* pause;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
	UInputAction* look;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	bool useCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	bool useFreeCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	FS_PlayerWalkSpeeds walkSpeeds;

	E_SpeedTypes currentSpeedType;

	FTimerHandle resetSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties
	APlayerCharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Move(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Sprint();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SprintCancel();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayerJump();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopPlayerJump();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Look(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PauseGame();

	bool GetUseFreeCamera() { return useFreeCamera; }

	UFUNCTION(BlueprintCallable)
	void SetSpeedByType(E_SpeedTypes speedType);

	void UpdateWalkSpeeds(FS_PlayerWalkSpeeds newSpeeds);

	void ResetSpeed();

	UInputAction* GetMove() { return move; }
};