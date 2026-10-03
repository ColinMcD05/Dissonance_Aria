// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MusicInfo.generated.h"

UENUM(BlueprintType)
enum class E_Genre : uint8
{
	None UMETA(DisplayName = "None"),
	Classical UMETA(DisplayName = "Orchestral"),
	Folk UMETA(DisplayName = "Folk"),
	Metal UMETA(DisplayName = "Metal"),
	Jazz UMETA(DisplayName = "Jazz")
};

UENUM(BlueprintType)
enum class E_SubGenre : uint8
{
	None UMETA(DisplayName = "None"),
	Brass UMETA(DisplayName = "Brass"),
	Choral UMETA(DisplayName = "Choral"),
	Irish UMETA(DisplayName = "Irish"),
	Banjo UMETA(DisplayName = "Banjo"),
	Heavy UMETA(DisplayName = "Heavy"),
	Gothic UMETA(DisplayName = "Gothic"),
	Bebop UMETA(DisplayName = "Bebop"),
	Blues UMETA(DisplayName = "Blues")
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
