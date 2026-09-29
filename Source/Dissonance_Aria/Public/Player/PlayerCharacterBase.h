

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "PlayerCharacterBase.generated.h"

UCLASS()
class DISSONANCE_ARIA_API APlayerCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacterBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
	UInputAction* move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
	UInputAction* jumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
	UInputAction* pause;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
	UInputAction* look;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs", meta = (AllowPrivateAccess = "true"))
	bool useFreeCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Move(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayerJump();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopPlayerJump();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Look(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PauseGame();

	bool GetUseFreeCamera() { return useFreeCamera; }
};
