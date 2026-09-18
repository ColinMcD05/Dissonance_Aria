// Fill out your copyright notice in the Description page of Project Settings.


#include "Inputs/BasicPlayerMapping.h"

UBasicPlayerMapping::UBasicPlayerMapping()
{
	if (!movement)
	{
		movement = SetUpAction<UMovement>("Movement");
	}

	SetMappings(movement, TMap<FKey, TArray<UInputModifier*>>{
						{EKeys::W, TArray<UInputModifier*>{CreateDefaultSubobject<UInputModifierSwizzleAxis>(TEXT("WSwizzleAxis"))}},
						{EKeys::S, TArray<UInputModifier*>{CreateDefaultSubobject<UInputModifierSwizzleAxis>(TEXT("SSwizzleAxis")), CreateDefaultSubobject<UInputModifierNegate>(TEXT("SNegate"))}},
						{EKeys::D, TArray<UInputModifier*>{}},
						{EKeys::A, TArray<UInputModifier*>{CreateDefaultSubobject<UInputModifierNegate>(TEXT("ANegate"))}},
						{EKeys::Gamepad_Left2D, TArray<UInputModifier*>{CreateDefaultSubobject<UInputModifierDeadZone>(TEXT("DeadZone"))}}
						});

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

//Set mappings to Input Action
void UBasicPlayerMapping::SetMappings(UInputAction* action, const TArray<FKey> keys)
{
	for (FKey key : keys)
	{
		MapKey(action, key);
	}
}

void UBasicPlayerMapping::SetMappings(UInputAction* action, const TMap<FKey, TArray<UInputModifier*>>& keys) 
{
	for (const TPair<FKey, TArray<UInputModifier*>> pair : keys)
	{
		FEnhancedActionKeyMapping& mapping = MapKey(action, pair.Key);

		for (UInputModifier* modifier : pair.Value)
		{
			if (modifier)
			{
				if (UInputModifierDeadZone* newModifier = Cast<UInputModifierDeadZone>(modifier))
				{
					if (UMovement* newAction = Cast<UMovement>(action))
					{
						newModifier->LowerThreshold = newAction->lowerThreshold;
						newModifier->UpperThreshold = newAction->higherThreshold;
					}
				}
				mapping.Modifiers.Add(modifier);
			}
		}
	}
}
