// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "BasicActions.generated.h"

//Start Create Input actions
UCLASS()
class DISSONANCE_ARIA_API UMovement : public UInputAction
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeadZone")
	float lowerThreshold = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeadZone")
	float higherThreshold = 1;

	UMovement();
};

UCLASS()
class DISSONANCE_ARIA_API UJump : public UInputAction
{
	GENERATED_BODY()
public:
	UJump();
};

UCLASS()
class DISSONANCE_ARIA_API USprint : public UInputAction
{
	GENERATED_BODY()
public:
	USprint();
};
//End Create Input actions