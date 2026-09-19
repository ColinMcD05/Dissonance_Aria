

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacterCombat.generated.h"

UCLASS()
class DISSONANCE_ARIA_API APlayerCharacterCombat : public ACharacter
{
	GENERATED_BODY()

private:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	APlayerCharacterCombat();

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
