// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inputs/CombatMapping.h"
#include "PlayerControllerCombat.generated.h"

/**
 * 
 */
UCLASS()
class DISSONANCE_ARIA_API APlayerControllerCombat : public APlayerController
{
	GENERATED_BODY()

private:
	void SetUpController();

protected:
	virtual void BeginPlay() override;

public:
};
