

#pragma once

#include "PlayerWalkSpeeds.generated.h"

UENUM(BlueprintType)
enum class E_SpeedTypes : uint8 
{
	Walking UMETA(DisplayName = "Walking"),
	Running UMETA(DisplayName = "Running"),
	Slowed UMETA(DisplayName = "Slowed")
};

USTRUCT(BlueprintType)
struct FS_PlayerWalkSpeeds
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	float walkSpeed ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	float runSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
	float slowed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackSpeed;
};