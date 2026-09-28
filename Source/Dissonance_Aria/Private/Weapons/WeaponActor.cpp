

#include "Weapons/WeaponActor.h"
#include "Player/PlayerCharacterCombat.h"

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
	capsuleComponent->SetGenerateOverlapEvents(false);
}

void AWeaponActor::InitializeWeapon(APlayerCharacterCombat* player, FS_WeaponInfo& newWeaponInfo)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	playerOwner = player;

	weaponInfo = newWeaponInfo;
	ChangeWeaponStats();
}


void AWeaponActor::Activate()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(true);
}

void AWeaponActor::Deactivate()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(false);
	capsuleComponent->SetGenerateOverlapEvents(false);
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

bool AWeaponActor::OverlappedActor_Implementation(AActor* enemyHit)
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
		damageInfo->damageAmount = stats.damage;
	}
	else
	{
		damageInfo->damageAmount = stats.damage;
	}
	
	playerOwner->GetCombatSystem()->DealDamage(enemyHit, *damageInfo);

	free(damageInfo);

	return true;
}

void AWeaponActor::SetWeaponInfo(FS_WeaponInfo& newWeaponInfo)
{
	weaponInfo = newWeaponInfo;
}

void AWeaponActor::PerformLightAttack_Implementation(const TArray<E_CombatActionType>& previousActions)
{
	int animationPosition = CalculateAnimationPosition(previousActions, E_CombatActionType::LightAttack);
	capsuleComponent->SetGenerateOverlapEvents(true);
	//Animation logic will go here, but I need animations first
}

void AWeaponActor::PerformHeavyAttack_Implementation(const TArray<E_CombatActionType>& previousActions)
{
	int animationPosition = CalculateAnimationPosition(previousActions, E_CombatActionType::HeavyAttack);
	capsuleComponent->SetGenerateOverlapEvents(true);
	//Animation logic will go here, but I need animations first
}

int AWeaponActor::CalculateAnimationPosition(const TArray<E_CombatActionType>& previousActions, E_CombatActionType currentAction)
{
	int currentPosition = -1;

	for (E_CombatActionType action : previousActions)
	{
		switch (action)
		{
			case E_CombatActionType::HeavyAttack:
				currentPosition += 2;
				break;
			case E_CombatActionType::LightAttack:
				currentPosition += 1;
		}
	}

	switch (currentAction)
	{
		case E_CombatActionType::HeavyAttack:
			currentPosition += 2;
			break;
		case E_CombatActionType::LightAttack:
			currentPosition += 1;
	}
	
	return currentPosition;
}

void AWeaponActor::ChangeWeaponStats()
{
	FS_Stats* newStats = statsTable[currentTuning]->FindRow<FS_Stats>(FName(*FString::FromInt(weaponInfo.level)), "", true);

	stats = *newStats;
}