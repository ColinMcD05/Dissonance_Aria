// Fill out your copyright notice in the Description page of Project Settings.


#include "InputActions/CombatActions.h"
#include "InputTriggers.h"

ULightAttack::ULightAttack()
{
	UInputTriggerPressed* trigger = CreateDefaultSubobject<UInputTriggerPressed>(TEXT("PressedTrigger"));
	Triggers.Add(trigger);
}

UHeavyAttack::UHeavyAttack()
{
	UInputTriggerPressed* trigger = CreateDefaultSubobject<UInputTriggerPressed>(TEXT("PressedTrigger"));
	Triggers.Add(trigger);
}

USidestep::USidestep()
{
	UInputTriggerPressed* trigger = CreateDefaultSubobject<UInputTriggerPressed>(TEXT("PressedTrigger"));
	Triggers.Add(trigger);
}

UChangeWeapon::UChangeWeapon()
{
	UInputTriggerPressed* trigger = CreateDefaultSubobject<UInputTriggerPressed>(TEXT("PressedTrigger"));
	Triggers.Add(trigger);
}

UActivateChangeTuning::UActivateChangeTuning()
{
	UInputTriggerHoldAndRelease* trigger = CreateDefaultSubobject
		<UInputTriggerHoldAndRelease>(TEXT("HoldAndRelease"));
	Triggers.Add(trigger);
}

UActivateSkills::UActivateSkills()
{
	UInputTriggerHoldAndRelease* trigger = CreateDefaultSubobject
		<UInputTriggerHoldAndRelease>(TEXT("HoldAndRelease"));
	Triggers.Add(trigger);
}