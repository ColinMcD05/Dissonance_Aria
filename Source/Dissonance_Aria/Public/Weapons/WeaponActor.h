

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/WeaponInfo.h"
#include "Components/CapsuleComponent.h"
#include "DamageSystem/DamageableInterface.h"
#include "WeaponActor.generated.h"

UCLASS()
class DISSONANCE_ARIA_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* weaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* capsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponData", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	int currentTuning;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	FS_WeaponInfo weaponInfo;

public:	
	AWeaponActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Hit")
	bool HitEnemy(AActor* hitEnemy);

	FS_WeaponInfo* GetWeaponInfo() { return &weaponInfo; }
};
