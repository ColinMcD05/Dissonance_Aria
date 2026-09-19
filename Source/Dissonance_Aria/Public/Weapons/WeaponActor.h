

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/WeaponInfo.h"
#include "Components/CapsuleComponent.h"
#include "DamageSystem/DamageableInterface.h"
#include "CombatSystem/CanAttackInterface.h"
#include "WeaponActor.generated.h"

class APlayerCharacterCombat;

UCLASS()
class DISSONANCE_ARIA_API AWeaponActor : public AActor, public ICanAttackInterface
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* weaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* capsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponData", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	int currentTuning;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	APlayerCharacterCombat* playerOwner;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	FS_WeaponInfo weaponInfo;

public:

	AWeaponActor();

	AWeaponActor(APlayerCharacterCombat* player);

	//Interface Functions
	virtual bool HitActor_Implementation(AActor* hitEnemy) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "WeaponData")
	FS_WeaponInfo GetWeaponInfo() { return weaponInfo; }
};
