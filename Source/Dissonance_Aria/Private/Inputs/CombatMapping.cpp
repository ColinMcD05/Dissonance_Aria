// Fill out your copyright notice in the Description page of Project Settings.


#include "Inputs/CombatMapping.h"
#include "InputModifiers.h"

UCombatMapping::UCombatMapping()
{
	//Start setting up input actions and create default mappiings
	if (!lightAttack) 
	{
		lightAttack = SetUpAction<ULightAttack>("LightAttack");
	}

	SetMappings(lightAttack, {EKeys::LeftMouseButton, EKeys::Gamepad_FaceButton_Left });
	
	
	if (!heavyAttack)
	{
		heavyAttack = SetUpAction<UHeavyAttack>("HeavyAttack");
	}
	
	SetMappings(heavyAttack, { EKeys::RightMouseButton, EKeys::Gamepad_FaceButton_Top });

	if (!sideStep)
	{
		sideStep = SetUpAction<USidestep>("Sidestep");
	}

	SetMappings(sideStep, { EKeys::F, EKeys::Gamepad_FaceButton_Right });

	if (!changeWeapon)
	{
		changeWeapon = SetUpAction<UChangeWeapon>("ChangeWeapon");
	}

	SetMappings(changeWeapon, { EKeys::C, EKeys::Gamepad_DPad_Down });

	if (!activateChangeTuning)
	{
		activateChangeTuning = SetUpAction<UActivateChangeTuning>("ActivateChangeTuning");
	}

	SetMappings(activateChangeTuning, { EKeys::T, EKeys::Gamepad_RightShoulder });

	if (!activateSkills)
	{
		activateSkills = SetUpAction<UActivateSkills>("ActivateSkills");
	}

	SetMappings(activateSkills, { EKeys::Q, EKeys::Gamepad_LeftShoulder });
}

//Initialize a new Input Action
template<typename T>
T* UCombatMapping::SetUpAction(FName name)
{
	return CreateDefaultSubobject<T>(name);
}

//Set mappings to Input Action
void UCombatMapping::SetMappings(UInputAction* action, const TArray<FKey> keys)
{
	for (FKey key : keys)
	{
		MapKey(action, key);
	}
}