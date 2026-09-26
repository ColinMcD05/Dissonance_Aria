

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
#include "AnimationInfo.generated.h"

USTRUCT(BlueprintType)
struct FS_AnimationInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* animMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float animLength;
};
