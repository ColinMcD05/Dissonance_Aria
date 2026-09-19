

#pragma once

#include "Music/MusicInfo.h"
#include "DamageInfo.generated.h"

//Damage Response type
UENUM(BlueprintType)
enum class EDamageResponse : uint8
{
	None UMETA(DisplayName = "None"),
	Stun  UMETA(DisplayName = "Stun"),
	Knockback UMETA(DisplayName = "Knockback"),
	Stagger UMETA(DisplayName = "Stagger")
};

//Information on type of damage
USTRUCT(BlueprintType)
struct FS_DamageInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float damageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	AActor* damageCauser;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool canInterrupt = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	EGenre genreAttack = EGenre::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	EDamageResponse damageResponse = EDamageResponse::None;
};