// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponInfo.generated.h"

ENUM(BlueprintType)
enum class EWeaponGenre : uint8
{
	Dagger,
	Sword,
	Greatsword,
	Scythe
}

ESTRUCT(BlueprintType)
struct class FWeaponInfo
{
	int level;
	EWeaponGenre genre;
	float damage;
	float speed;
	int maxHP;
	float sideStepDistance;
}