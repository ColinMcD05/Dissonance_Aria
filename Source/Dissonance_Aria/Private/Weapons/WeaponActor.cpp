

#include "Weapons/WeaponActor.h"

// Sets default values
AWeaponActor::AWeaponActor()
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

bool AWeaponActor::HitEnemy_Implementation(AActor* enemyHit)
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
	
	enemy->Execute_TakeDamage(Cast<UObject>(enemy), *damageInfo);

	free(damageInfo);

	return true;
}