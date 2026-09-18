// Fill out your copyright notice in the Description page of Project Settings.


#include "Inputs/BasicPlayerMapping.h"

UBasicPlayerMapping::UBasicPlayerMapping()
{
	//Sets up all input actions
	if (!movement)
	{
		movement = CreateObject<UMovement>("Movement");
	}

	SetMappings(movement, TMap<FKey, TArray<UInputModifier*>>{
						{EKeys::W, TArray<UInputModifier*>{CreateObject<UInputModifierSwizzleAxis>("WSwizzleAxis")}},
						{EKeys::S, TArray<UInputModifier*>{CreateObject<UInputModifierSwizzleAxis>("SSwizzleAxis"), CreateObject<UInputModifierNegate>("SNegate")}},
						{EKeys::D, TArray<UInputModifier*>{}},
						{EKeys::A, TArray<UInputModifier*>{CreateObject<UInputModifierNegate>("ANegate")}},
						{EKeys::Gamepad_Left2D, TArray<UInputModifier*>{CreateObject<UInputModifierDeadZone>("DeadZone"), CreateObject<UInputModifierScalar>("Scalar")}}
						});

	if (!jump)
	{
		jump = CreateObject<UJump>("Jump");
	}

	SetMappings(jump, { EKeys::SpaceBar, EKeys::Gamepad_FaceButton_Bottom });

	if (!sprint)
	{
		sprint = CreateObject<USprint>("Sprint");
	}

	SetMappings(sprint, { EKeys::Gamepad_LeftThumbstick, EKeys::LeftShift });
}

//Creates default Subobject of Type T
template<typename T>
T* UBasicPlayerMapping::CreateObject(FName name)
{
	return CreateDefaultSubobject<T>(name);
}

//Set mappings to Input Action
void UBasicPlayerMapping::SetMappings(UInputAction* action, const TArray<FKey> keys)
{
	for (FKey key : keys)
	{
		MapKey(action, key);
	}
}

//Set mappings to Input Action and modifiers
void UBasicPlayerMapping::SetMappings(UInputAction* action, const TMap<FKey, TArray<UInputModifier*>>& keys) 
{
	//Loops through the keys
	for (const TPair<FKey, TArray<UInputModifier*>> pair : keys)
	{
		FEnhancedActionKeyMapping& mapping = MapKey(action, pair.Key);

		//For each key, loop through the modifier array
		for (UInputModifier* modifier : pair.Value)
		{
			//Check if modifier exists
			if (modifier)
			{
				//Check if modifier is dead zone and action is movement
				if (UInputModifierDeadZone* newModifier = Cast<UInputModifierDeadZone>(modifier))
				{
					if (UMovement* newAction = Cast<UMovement>(action))
					{
						//Assign thresholds
						newModifier->LowerThreshold = newAction->lowerThreshold;
						newModifier->UpperThreshold = newAction->higherThreshold;
					}
				}
				//Add modifier
				mapping.Modifiers.Add(modifier);
			}
		}
	}
}

//Getters
UMovement* UBasicPlayerMapping::GetMovement()
{
	return movement;
}

UJump* UBasicPlayerMapping::GetJump()
{
	return jump;
}

USprint* UBasicPlayerMapping::GetSprint()
{
	return sprint;
}