

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PDA_Equipment.generated.h"

UENUM()
enum class E_StatType : uint8
{
	Health UMETA(DisplayName = "Health"),
	Damage UMETA(DisplayName = "Damage"),
	Experience UMETA(DisplayName = "Experience"),
	Gold UMETA(DisplayName = "Gold")
};

UENUM()
enum class E_AdditiveType : uint8
{
	AddSub UMETA(DisplayName = "Add/Sub"),
	MultDiv UMETA(DisplayName = "Mult/Div")
};

UENUM()
enum class E_ItemType : uint8
{
	Equipment UMETA(DisplayName = "Equipment"),
	Potion UMETA(DisplayName = "Potion"),
	Song UMETA(DisplayName = "Song"),
	Tuning UMETA(DisplayName = "Tuning")
};

USTRUCT(BlueprintType)
struct FS_StatAffected
{
	GENERATED_BODY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatAffected")
	E_AdditiveType additionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatAffected")
	E_StatType affectedStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatAffected")
	float affectedAmount;
};
/**
 * 
 */
UCLASS()
class DISSONANCE_ARIA_API UPDA_InventoryPiece : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FString> description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	E_ItemType itemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int amount;
};

UCLASS()
class DISSONANCE_ARIA_API UPDA_Equipment : public UPDA_InventoryPiece
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FS_StatAffected> affectedStats;
};