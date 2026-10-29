

#include "Weapons/WeaponActor.h"
#include "Player/PlayerCharacterCombat.h"
#include "GameInfo/DAGameStateCombat.h"
#include "BasicFunctions/GetGameInfo.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	//Setup basic components
	//Setup Mesh
	weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

	//Set RootComponent
	RootComponent = weaponMesh;

	weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	weaponMesh->SetSimulatePhysics(false);

	SetActorEnableCollision(false);
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	if (ADAGameStateCombat* gameState = GameInfoUtilities::GetDAGameState<ADAGameStateCombat>(this))
	{
		gameState->OnCombatEnd.AddDynamic(this, &AWeaponActor::CombatEnd);
	}
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region Weapon Creation
//Activates the weapon
void AWeaponActor::Activate()
{
	//Shows weapon
	SetActorHiddenInGame(false);

	activated = true;
}

//Deactivate weapon
void AWeaponActor::Deactivate()
{
	//Hides weapon and disables collision
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	//Deactivates weapon
	activated = false;
}

//Initializes the weapon
void AWeaponActor::InitializeWeapon(APlayerCharacterCombat* player, FS_WeaponInfo& newWeaponInfo)
{
	//Setup ticking
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	//Set plater owner
	playerOwner = player;

	//Assign weapon info
	weaponInfo = newWeaponInfo;

	//Get level
	/*if (statsTable.Num() > 0 && statsTable[0])
	{
		weaponInfo.level = FMath::Clamp(weaponInfo.level, 1, statsTable[0]->GetRowNames().Num());
	}*/
}
#pragma endregion

#pragma region Attacking
//Light attacks
float AWeaponActor::PerformLightAttack_Implementation(const TArray<E_CombatActionType>& previousActions)
{
	lastAttack = E_CombatActionType::LightAttack;
	//Animation logic will go here, but I need animations first
	return 1.5;
}

//Heavy attacks
float AWeaponActor::PerformHeavyAttack_Implementation(const TArray<E_CombatActionType>& previousActions)
{
	lastAttack = E_CombatActionType::HeavyAttack;
	//Animation logic will go here, but I need animations first
	return 1.5;
}

/*
void AWeaponActor::StartChargeAttack_Implementation()
{
	originalLocation = weaponMesh->GetRelativeLocation();
	speed = 200 / 0.8;
	ChargeAttack();
}

void AWeaponActor::ChargeAttack_Implementation()
{
	if (attackCharged)
	{
		playerOwner->ChargeReady();
		attackCharged = false;
	}

	if (originalLocation.Y - 100 <= weaponMesh->GetRelativeLocation().Y)
	{
		FVector addedVector = FVector(0, -speed * GetWorld()->DeltaTimeSeconds, 0);
		weaponMesh->AddRelativeLocation(addedVector);
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AWeaponActor::ChargeAttack);
	}
	else
	{
		playerOwner->ChargeReady();
	}
}
*/

//Starts the sphere tracing
void AWeaponActor::StartSphereTrace_Implementation()
{
	attacking = true;
	PerformSphereTrace();
}

//Performs sphere tracing
void AWeaponActor::PerformSphereTrace_Implementation()
{
	//Checks if currently attacking
	if (attacking)
	{
		//Get socket locations
		FVector startLocation = weaponMesh->GetSocketLocation("START");
		FVector endLocation = weaponMesh->GetSocketLocation("END");

		//Hit actors
		FHitResult hit;

		//Do spheretrace
		if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), startLocation, endLocation, radius,
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4), false, {playerOwner}, EDrawDebugTrace::ForOneFrame, hit, true))
		{
			if (hit.GetActor())
			{
				Execute_OverlappedActor(this, hit.GetActor());
			}
		}
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AWeaponActor::PerformSphereTrace);
	}
	else
	{
		//Empty enemy array
		hitEnemies.Empty();
	}
}
#pragma endregion

#pragma region Weapon Info
void AWeaponActor::SetWeaponInfo(FS_WeaponInfo& newWeaponInfo)
{
	weaponInfo = newWeaponInfo;
}

void AWeaponActor::ChangeWeaponStats(FS_Stats* newStats)
{
	stats = *newStats;
}
#pragma endregion

#pragma region EndCombat
void AWeaponActor::CombatEnd(int gainedExp, int gold)
{
	/*
	weaponInfo.exp.currentExperience += gainedExp;
	if (statsTable.Num() == 0 && statsTable[0] == nullptr)
	{

		return;
	}
	if (!playerOwner)
	{

		return;
	}
	playerOwner->UpdateInventoryWeaponInfo(weaponInfo, statsTable[0]->GetRowNames().Num());
	*/
}
#pragma endregion

#pragma region AttackInterface
bool AWeaponActor::OverlappedActor_Implementation(AActor* enemyHit)
{
	if (!enemyHit || !enemyHit->GetClass()->ImplementsInterface(
		UDamageableInterface::StaticClass()))
	{
		return false;
	}

	if (hitEnemies.Contains(enemyHit))
		return false;

	FS_DamageInfo damageInfo;
	damageInfo.damageCauser = GetOwner();
	damageInfo.genreAttack = currentGenre;

	damageInfo.damageAmount = stats.damage;

	playerOwner->GetCombatSystem()->DealDamage(enemyHit, damageInfo);

	hitEnemies.Add(enemyHit);

	return true;
}
#pragma endregion