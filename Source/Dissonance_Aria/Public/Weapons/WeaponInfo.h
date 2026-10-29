// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Music/MusicInfo.h"
#include "Player/PlayerWalkSpeeds.h"
#include "WeaponInfo.generated.h"

class AWeaponActor;

UENUM(BlueprintType)
enum class E_QTEInput : uint8
{
	Cancel = 0 UMETA(DisplayName = "Cancel QTE"),
	Input1 = 1 UMETA(DisplayName = "West Gamepad Button/Left Arrow"),
	Input2 = 2 UMETA(DisplayName = "North Gamepad Button/Right Arrow"),
	Input3 = 3 UMETA(DisplayName = "South Gamepad Button/Up Arrow")
};

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tunings")
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tunings")
	E_Genre genre;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tunings")
	UDataTable* statsTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tunings")
	TArray<E_QTEInput> qteInputs;
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