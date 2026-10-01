

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponInfo.h"
#include "SaveInfo.generated.h"

//Information about weapons that need to be saved
USTRUCT(BlueprintType)
struct FS_WeaponSave
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveInfo")
	int level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveInfo")
	bool isAcquired;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveInfo")
	float toleranceMeter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveInfo")
	E_WeaponType weaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveInfo")
	FS_Experience experience;
};

//Information that gets taken from the inventory and put back in
USTRUCT(BlueprintType)
struct FS_InventorySave
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveInfo")
	TArray<FS_WeaponSave> weapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveInfo")
	int weaponOneIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveInfo")
	int weaponTwoIndex;
};
