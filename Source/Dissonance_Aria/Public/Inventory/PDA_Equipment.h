

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
	E_StatType affectedStat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float affectedAmount;
};