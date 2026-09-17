// Fill out your copyright notice in the Description page of Project Settings.


#include "Inputs/BasicPlayerMapping.h"

UBasicPlayerMapping::UBasicPlayerMapping()
{
	if (!movement)
	{
		movement = SetUpAction<UMovement>("Movement");
	}
	
	SetMappings(movement, { EKeys::W, EKeys::S, EKeys::A, EKeys::D, EKeys::Gamepad_Left2D});

	if (!jump)
	{
		jump = SetUpAction<UJump>("Jump");
	}

	SetMappings(jump, { EKeys::SpaceBar, EKeys::Gamepad_FaceButton_Bottom });

	if (!sprint)
	{
		sprint = SetUpAction<USprint>("Sprint");
	}

	SetMappings(sprint, { EKeys::Gamepad_LeftThumbstick, EKeys::LeftShift });
}

template<typename T>
T* UBasicPlayerMapping::SetUpAction(FName name)
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
