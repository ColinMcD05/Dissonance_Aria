// Fill out your copyright notice in the Description page of Project Settings.


#include "Inputs/CombatMapping.h"
#include "InputModifiers.h"

UCombatMapping::UCombatMapping()
{
	//Start setting up input actions and create default mappiings
	if (!lightAttack) 
	{
		lightAttack = Super::SetUpAction<ULightAttack>("LightAttack");
	}

	Super::SetMappings(lightAttack, {EKeys::LeftMouseButton, EKeys::Gamepad_FaceButton_Left });
	
	
	if (!heavyAttack)
	{
		heavyAttack = Super::SetUpAction<UHeavyAttack>("HeavyAttack");
	}
	
	Super::SetMappings(heavyAttack, { EKeys::RightMouseButton, EKeys::Gamepad_FaceButton_Top });

	if (!sideStep)
	{
		sideStep = Super::SetUpAction<USidestep>("Sidestep");
	}

	Super::SetMappings(sideStep, { EKeys::F, EKeys::Gamepad_FaceButton_Right });

	if (!changeWeapon)
	{
		changeWeapon = Super::SetUpAction<UChangeWeapon>("ChangeWeapon");
	}

	SetMappings(changeWeapon, { EKeys::C, EKeys::Gamepad_DPad_Down });

	if (!activateChangeTuning)
	{
		activateChangeTuning = Super::SetUpAction<UActivateChangeTuning>("ActivateChangeTuning");
	}

	Super::SetMappings(activateChangeTuning, { EKeys::T, EKeys::Gamepad_RightShoulder });

	if (!activateSkills)
	{
		activateSkills = Super::SetUpAction<UActivateSkills>("ActivateSkills");
	}

	Super::SetMappings(activateSkills, { EKeys::Q, EKeys::Gamepad_LeftShoulder });
}