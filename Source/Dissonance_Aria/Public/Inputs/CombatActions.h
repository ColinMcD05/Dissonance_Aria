// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "CombatActions.generated.h"

//Start Create Input actions
UCLASS()
class DISSONANCE_ARIA_API ULightAttack : public UInputAction
{
	GENERATED_BODY()

public:
	ULightAttack();
};

UCLASS()
class DISSONANCE_ARIA_API UHeavyAttack : public UInputAction
{
	GENERATED_BODY()
public:
	UHeavyAttack();
};

UCLASS()
class DISSONANCE_ARIA_API USidestep : public UInputAction
{
	GENERATED_BODY()
public:
	USidestep();
};

UCLASS()
class DISSONANCE_ARIA_API UChangeWeapon : public UInputAction
{
	GENERATED_BODY()
public:
	UChangeWeapon();
};

UCLASS()
class DISSONANCE_ARIA_API UActivateChangeTuning : public UInputAction
{
	GENERATED_BODY()
public:
	UActivateChangeTuning();
};

UCLASS()
class DISSONANCE_ARIA_API UActivateSkills : public UInputAction
{
	GENERATED_BODY()
public:
	UActivateSkills();
};

UCLASS()
class DISSONANCE_ARIA_API USwitchEnemy : public UInputAction
{
	GENERATED_BODY()
public:
	USwitchEnemy();
};
//End Create Input actions