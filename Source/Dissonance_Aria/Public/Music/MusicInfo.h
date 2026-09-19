// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MusicInfo.generated.h"

UENUM(BlueprintType)
enum class EGenre : uint8
{
	None UMETA(DisplayName = "None"),
	Metal UMETA(DisplayName = "Metal"),
	Heavy UMETA(DisplayName = "Heavy"),
	Gothic UMETA(DisplayName = "Gothic"),
	Classical UMETA(DisplayName = "Orchestral"),
	Brass UMETA(DisplayName = "Brass"),
	Choral UMETA(DisplayName = "Choral")
};

USTRUCT(BlueprintType)
struct FSongInfo
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
	EGenre genre;
};
