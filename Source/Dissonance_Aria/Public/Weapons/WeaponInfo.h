// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Music/MusicInfo.h"
#include "WeaponInfo.generated.h"

class AWeaponActor;

//Enumerator for each weapon type
UENUM(BlueprintType)
enum class E_WeaponType : uint8
{
	Greatsword UMETA(DisplayName = "Greatsword"),
	Sword UMETA(DisplayName = "Sword"),
	Scythe UMETA(DisplayName = "Scythe"),
	Dagger UMETA(DisplayName = "Dagger")
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
struct FS_Stats : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int maxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float sidestepDistance;
};

USTRUCT(BlueprintType)
struct FS_Tuning
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (ClampMin = "0", ClampMax = "1"))
	float toleranceMeter = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tunings")
	E_SubGenre subGenre;
};

//Struct for all important weapon info. This is what weapons reference
USTRUCT(BlueprintType)
struct FS_WeaponInfo
{
	GENERATED_BODY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	int level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (ClampMin = "0", ClampMax = "1"))
	float toleranceMeter = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	E_WeaponType weaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	FS_Experience exp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	E_Genre genre;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TArray<FS_Tuning> tunings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TSubclassOf<AWeaponActor> weaponActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	bool acquired = false;
};

USTRUCT(BlueprintType)
struct FS_LevelData : public FTableRowBase
{
	GENERATED_BODY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exp")
	int requiredExp;
};