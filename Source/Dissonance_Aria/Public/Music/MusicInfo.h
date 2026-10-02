// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MusicInfo.generated.h"

UENUM(BlueprintType)
enum class E_Genre : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Classical = 1 UMETA(DisplayName = "Orchestral"),
	Folk = 4 UMETA(DisplayName = "Folk"), 
	Jazz = 7 UMETA(DisplayName = "Jazz"),
	Metal = 10 UMETA(DisplayName = "Metal")
};

UENUM(BlueprintType)
enum class E_SubGenre : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Choral = 1 UMETA(DisplayName = "Choral"),
	Brass = 2 UMETA(DisplayName = "Brass"),
	Banjo = 1 UMETA(DisplayName = "Banjo"),
	Celtic = 2 UMETA(DisplayName = "Irish"),
	Bebop = 1 UMETA(DisplayName = "Bebop"),
	Blues = 2 UMETA(DisplayName = "Blues"),
	Heavy = 1 UMETA(DisplayName = "Heavy"),
	Gothic = 2 UMETA(DisplayName = "Gothic")
};

USTRUCT(BlueprintType)
struct FS_SongInfo
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Song")
	USoundWave* songAudio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Song")
	FString name = FString(TEXT(""));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Song")
	int bpm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Song")
	E_Genre genre;
};
