

#pragma once

#include "PlayerWalkSpeeds.generated.h"

USTRUCT(BlueprintType)
struct FS_PlayerWalkSpeeds
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	float walkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	float runSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	float slowed;
};