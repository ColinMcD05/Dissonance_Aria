

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponInfo.h"
#include "Inventory/PDA_Equipment.h"
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
	int gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveInfo")
	TArray<FS_Tuning> equppiedTunings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveInfo")
	TArray<FS_Tuning> allTunings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveInfo")
	TArray<UPDA_Equipment*> gainedEquipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveInfo")
	TArray<UPDA_Equipment*> equipedEquipment;
};
