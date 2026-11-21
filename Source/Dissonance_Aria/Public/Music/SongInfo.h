

#pragma once

#include "Engine/DataAsset.h"
#include "CoreMinimal.h"
#include "Inventory/PDA_Equipment.h"
#include "SongInfo.generated.h"

UCLASS()
class DISSONANCE_ARIA_API UPDA_Notes : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notes")
	FString note;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notes")
	UTexture2D* image;
};

UCLASS()
class DISSONANCE_ARIA_API UPDA_SongInfo : public UPDA_InventoryPiece
{

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Song")
	TArray<UPDA_Notes*> notes;
};
