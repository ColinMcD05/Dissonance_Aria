// Fill out your copyright notice in the Description page of Project Settings.


#include "Inputs/BasicActions.h"

UMovement::UMovement()
{
	ValueType = EInputActionValueType::Axis2D;
}

UJump::UJump()
{
	UInputTriggerPressed* trigger = CreateDefaultSubobject<UInputTriggerPressed>(TEXT("PressedTrigger"));
	Triggers.Add(trigger);
}

USprint::USprint()
{
	UInputTriggerPulse* trigger = CreateDefaultSubobject<UInputTriggerPulse>(TEXT("PressedTrigger"));
	Triggers.Add(trigger);
}