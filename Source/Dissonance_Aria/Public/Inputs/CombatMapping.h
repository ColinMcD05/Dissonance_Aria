// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatActions.h"
#include "InputMappingContext.h"
#include "BasicPlayerMapping.h"
#include "CombatMapping.generated.h"

/**
 * 
 */
UCLASS()
class DISSONANCE_ARIA_API UCombatMapping : public UBasicPlayerMapping
{
	GENERATED_BODY()

private:
	//Input actions
	UPROPERTY()
	ULightAttack* lightAttack;

	UPROPERTY()
	UHeavyAttack* heavyAttack;

	UPROPERTY()
	USidestep* sideStep;

	UPROPERTY()
	UChangeWeapon* changeWeapon;

	UPROPERTY()
	UActivateChangeTuning* activateChangeTuning;

	UPROPERTY()
	UActivateSkills* activateSkills;

public:
	UCombatMapping();
};
