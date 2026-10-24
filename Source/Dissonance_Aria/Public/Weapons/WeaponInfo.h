// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Music/MusicInfo.h"
#include "Player/PlayerWalkSpeeds.h"
#include "WeaponInfo.generated.h"

class AWeaponActor;

//Struct to hold all the experience
USTRUCT(BlueprintType)
struct FS_Experience
{
	GENERATED_BODY();
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
	int currentExperience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
	int storedExperience;
};

//Struct holding the important stats for weapons
USTRUCT(BlueprintType)
struct FS_Stats : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int maxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FS_PlayerWalkSpeeds speeds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float sidestepDistance;
};

//Struct holding the tuning information
USTRUCT(BlueprintType)
struct FS_Tuning
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tunings", meta = (ClampMin = "0", ClampMax = "1"))
	float toleranceMeter = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tunings")
	E_Genre genre;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tunings")
	FS_Stats stats;
};

//Struct for all important weapon info. This is what weapons reference
USTRUCT(BlueprintType)
struct FS_WeaponInfo
{
	GENERATED_BODY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	int level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	FS_Experience exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TSubclassOf<AWeaponActor> weaponActor;
};

//Struct for levels
USTRUCT(BlueprintType)
struct FS_LevelData : public FTableRowBase
{
	GENERATED_BODY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exp")
	int requiredExp;
};