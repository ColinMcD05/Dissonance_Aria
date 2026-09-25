

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/WeaponInfo.h"
#include "Components/CapsuleComponent.h"
#include "DamageSystem/DamageableInterface.h"
#include "CombatSystem/HurtBoxInterface.h"
#include "Input/InputActions/CombatActionBase.cpp"
#include "WeaponActor.generated.h"

class APlayerCharacterCombat;

UCLASS()
class DISSONANCE_ARIA_API AWeaponActor : public AActor, public IHurtBoxInterface
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

	TArray<E_CombatActionType> previousActions;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FS_WeaponInfo* weaponInfo;

public:

	AWeaponActor();

	void InitializeWeapon(APlayerCharacterCombat* player, FS_WeaponInfo* newWeaponInfo);

	//Interface Functions
	virtual bool OverlappedActor_Implementation(AActor* hitEnemy) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "WeaponData")
	FS_WeaponInfo& GetWeaponInfo() { return *weaponInfo; }

	UFUNCTION(BlueprintCallable, Category = "WeaponData")
	void SetWeaponInfo(FS_WeaponInfo& newWeaponInfo);
};
