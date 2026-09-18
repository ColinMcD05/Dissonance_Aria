// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "BasicActions.h"
#include "BasicPlayerMapping.generated.h"

/**
 * 
 */
UCLASS()
class DISSONANCE_ARIA_API UBasicPlayerMapping : public UInputMappingContext
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	UMovement* movement;

	UPROPERTY()
	UJump* jump;

	UPROPERTY()
	USprint* sprint;

public:
	UBasicPlayerMapping();

	//Initialize a new Input Action
	template<typename T>
	T* SetUpAction(FName name);

	//Set mappings to Input Action
	void SetMappings(UInputAction* action, const TArray<FKey> keys);

	void SetMappings(UInputAction* action, const TMap<FKey, TArray<UInputModifier*>>& keys);
};


template<typename T>
T* UBasicPlayerMapping::SetUpAction(FName name)
{
	return CreateDefaultSubobject<T>(name);
}