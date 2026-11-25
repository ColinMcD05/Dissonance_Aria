

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Dialogue.generated.h"

USTRUCT(BlueprintType)
struct FS_DialogueChunks
{
	GENERATED_BODY()

public:
	//Dialogue thats grouped together and whether it is interactable or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TMap<FString, bool> paragraph;
};

UCLASS()
class DISSONANCE_ARIA_API UPDA_DialogueInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	//Paragraphs of dialogue put together.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FS_DialogueChunks> dialogueChunks;
};