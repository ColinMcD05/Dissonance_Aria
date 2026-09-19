

#include "Weapons/WeaponActor.h"
#include "Player/PlayerCharacterCombat.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;
}

AWeaponActor::AWeaponActor(APlayerCharacterCombat* player)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	//Setup basic components
	//Setup Mesh
	weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	weaponMesh->SetupAttachment(RootComponent);

	//Setup Capsule
	capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitBox"));
	capsuleComponent->SetupAttachment(weaponMesh);
	capsuleComponent->SetGenerateOverlapEvents(true);

	playerOwner = player;
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponActor::OnPickupOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	HitActor(otherActor);
}

bool AWeaponActor::HitActor_Implementation(AActor* enemyHit)
{
	IDamageableInterface* enemy = Cast<IDamageableInterface>(enemyHit);

	if (!enemy)
	{
		return false;
	}

	FS_DamageInfo* damageInfo = new FS_DamageInfo();
	damageInfo->damageCauser = GetOwner();
	damageInfo->genreAttack = weaponInfo.genre;
	if (currentTuning != 0)
	{
		damageInfo->damageAmount = weaponInfo.tunings[currentTuning - 1].damage;
	}
	else
	{
		damageInfo->damageAmount = weaponInfo.damage;
	}
	
	playerOwner->GetCombatSystem()->DealDamage(enemyHit, damageInfo);

	free(damageInfo);

	return true;
}