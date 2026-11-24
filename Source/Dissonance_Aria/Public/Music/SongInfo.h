

#pragma once

#include "Engine/DataAsset.h"
#include "CoreMinimal.h"
#include "Inventory/PDA_Equipment.h"
#include "FMODEvent.h"
#include "SongInfo.generated.h"

UENUM(BlueprintType)
enum class E_NoteTypes : uint8
{
	C UMETA(DisplayName = "C"),
	D UMETA(DisplayName = "D"),
	E UMETA(DisplayName = "E"),
	F UMETA(DisplayName = "F"),
	G UMETA(DisplayName = "G"),
	A UMETA(DisplayName = "A"),
	B UMETA(DisplayName = "B")
};

UCLASS()
class DISSONANCE_ARIA_API UPDA_Notes : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Notes")
	FString note;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Notes")
	E_NoteTypes noteType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Notes")
	UTexture2D* image;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Notes")
	UFMODEvent* sound;
};

UCLASS()
class DISSONANCE_ARIA_API UPDA_SongInfo : public UPDA_InventoryPiece
{

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Song")
	TArray<UPDA_Notes*> notes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Song")
	UFMODEvent* song;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Song")
	float length;
};
