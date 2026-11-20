

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
class DISSONANCE_ARIA_API UPDA_Equipment : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FString> description;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FS_StatAffected> affectedStats;
};