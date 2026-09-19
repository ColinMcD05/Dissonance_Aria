// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Music/MusicInfo.h"
#include "WeaponInfo.generated.h"

//Enumerator for each weapon type
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Dagger UMETA(DisplayName = "Dagger"),
	Sword UMETA(DisplayName = "Sword"),
	Greatsword UMETA(DisplayName = "Greatsword"),
	Scythe UMETA(DisplayName = "Scythe")
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


USTRUCT(BlueprintType)
struct FS_Tuning
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	int maxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	float damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	float speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	float sidestepDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tunings")
	ESubGenre genre;
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
	int maxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	float damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	float speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	float sidestepDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	EWeaponType weaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	FS_Experience exp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	EGenre genre;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TArray<FS_Tuning> tunings;
};