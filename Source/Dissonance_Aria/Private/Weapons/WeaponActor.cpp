

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

	//Set up weapon
	currentTuning = 0;
	ChangeWeaponStats();
	activated = true;

	//Lowers tolerance
	LowerTolerance();
	if (!raisingTolerance)
	{
		RaiseTuningTolerance();
	}
}

//Deactivate weapon
void AWeaponActor::Deactivate()
{
	//Hides weapon and disables collision
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	//Deactivates weapon
	activated = false;
	RaiseTolerance();
	GetWorld()->GetTimerManager().SetTimer(raiserTimer, this, &AWeaponActor::RaiseTolerance, 5, false);
	currentTuning = 0;

	if (!raisingTolerance)
	{
		RaiseTuningTolerance();
	}
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
	//Reset tolerance meter for tunings and weapons
	weaponInfo.toleranceMeter = 1;

	for (FS_Tuning tolerance : weaponInfo.tunings)
	{
		tolerance.toleranceMeter = 1;
	}

	//Get level
	if (statsTable.Num() > 0 && statsTable[0])
	{
		weaponInfo.level = FMath::Clamp(weaponInfo.level, 1, statsTable[0]->GetRowNames().Num());
	}

	//Change stats
	ChangeWeaponStats();
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

#pragma region Tolerance
void AWeaponActor::LowerTolerance()
{
	if (activated)
	{
		if (weaponInfo.toleranceMeter > 0)
		{
			weaponInfo.toleranceMeter -= GetWorld()->DeltaTimeSeconds / timeToDrainTolerance;
			weaponInfo.toleranceMeter = FMath::Clamp(weaponInfo.toleranceMeter, 0, 1);
			if (currentTuning > 0)
			{
				if (weaponInfo.tunings.Num() == currentTuning)
				{
					weaponInfo.tunings[currentTuning - 1].toleranceMeter -= GetWorld()->DeltaTimeSeconds / timeToDrainTuning;
					weaponInfo.tunings[currentTuning - 1].toleranceMeter = FMath::Clamp(weaponInfo.tunings[currentTuning - 1].toleranceMeter, 0, 1);
					if (weaponInfo.tunings[currentTuning - 1].toleranceMeter <= 0)
					{
						currentTuning = 0;
						ChangeWeaponStats();
						if (playerOwner)
						{
							if (playerOwner->GetWeaponsSystem())
							{
								playerOwner->GetWeaponsSystem()->OnTuningSwapped.Broadcast(0);
								playerOwner->GetWeaponsSystem()->OnChangeMusic.Broadcast(static_cast<int32>(weaponInfo.genre));
							}
						}
					}
				}
			}
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AWeaponActor::LowerTolerance);
		}
		else
		{
			depleted = true;
			currentTuning = 0;
			ChangeWeaponStats();

		}
	}
}

void AWeaponActor::RaiseTolerance()
{
	if (!activated)
	{
		if (weaponInfo.toleranceMeter < 1)
		{
			weaponInfo.toleranceMeter += GetWorld()->DeltaTimeSeconds / timeToFillTolerance;
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AWeaponActor::RaiseTolerance);
			weaponInfo.toleranceMeter = FMath::Clamp(weaponInfo.toleranceMeter, 0, 1);
		}
		else if (depleted)
		{
			stats.damage /= 0.75f;
			stats.sidestepDistance /= 0.75f;
			depleted = false;
			ChangeWeaponStats();

		}
	}
}


void AWeaponActor::RaiseTuningTolerance()
{
	if (depleted && activated)
	{
		return;
	}
	raisingTolerance = true;
	bool checks[2] = { false, false };
	for (int i = 1; i <= weaponInfo.tunings.Num(); i++)
	{
		if (checks[i])
		{
			continue;
		}
		if (i == currentTuning)
		{
			checks[i] = true;
			continue;
		}
		if (weaponInfo.tunings[i - 1].toleranceMeter < 1)
		{
			weaponInfo.tunings[i - 1].toleranceMeter += GetWorld()->DeltaTimeSeconds / timeToDrainTolerance;
			weaponInfo.tunings[i - 1].toleranceMeter = FMath::Clamp(weaponInfo.tunings[i - 1].toleranceMeter, 0, 1);
		}
		else
		{
			checks[i] = true;
		}
	}
	if (checks[0])
	{
		raisingTolerance = false;
		return;
	}

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AWeaponActor::RaiseTuningTolerance);
}

float AWeaponActor::GetCurrentWeaponTolerance()
{
	return weaponInfo.toleranceMeter;
}

//When returning -1, there is no active tuning
float AWeaponActor::GetCurrentTuningTolerance()
{
	if (currentTuning == 0 || currentTuning > weaponInfo.tunings.Num())
	{
		return -1;
	}

	return weaponInfo.tunings[currentTuning - 1].toleranceMeter;
}
#pragma endregion

#pragma region Weapon Info
	void AWeaponActor::SetWeaponInfo(FS_WeaponInfo& newWeaponInfo)
	{
		weaponInfo = newWeaponInfo;
	}

	void AWeaponActor::ChangeWeaponStats()
	{
		if (currentTuning < statsTable.Num() && statsTable[currentTuning])
		{
			int nextLevel = FMath::Clamp(weaponInfo.level, 1, statsTable[currentTuning]->GetRowNames().Num());

			FS_Stats* newStats = statsTable[currentTuning]->FindRow<FS_Stats>(FName(*FString::FromInt(weaponInfo.level)), "", true);

			if (newStats)
			{
				stats = *newStats;
			}
			if (newStats && depleted)
			{
				stats.damage *= depletionAmount;
				stats.sidestepDistance *= depletionAmount;

			}
		}
	}
#pragma endregion

#pragma region Tunings
	int AWeaponActor::SwapTuning(int newTuning)
	{
		if (newTuning > weaponInfo.tunings.Num() || depleted)
		{
			return -1;
		}

		if (newTuning == currentTuning)
		{
			currentTuning = 0;
			ChangeWeaponStats();
			return 0;
		}

		if (weaponInfo.tunings[newTuning - 1].toleranceMeter < 1)
		{
			return -1;
		}

		currentTuning = newTuning;
		if (!raisingTolerance)
		{
			RaiseTuningTolerance();
		}
		ChangeWeaponStats();

		return currentTuning;
	}

	int AWeaponActor::CanTune(int tuning)
	{
		if (tuning > weaponInfo.tunings.Num() || depleted)
		{
			return -1;
		}

		if (tuning == currentTuning)
		{
			return 0;
		}

		if (weaponInfo.tunings[tuning - 1].toleranceMeter < 1)
		{
			return -1;
		}

		return 1;
	}

	int AWeaponActor::GetCurrentTuning()
	{
		return currentTuning;
	}
#pragma endregion

#pragma region EndCombat
void AWeaponActor::CombatEnd(int gainedExp)
{
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
	damageInfo.genreAttack = weaponInfo.genre;

	damageInfo.damageAmount = stats.damage;

	playerOwner->GetCombatSystem()->DealDamage(enemyHit, damageInfo);

	hitEnemies.Add(enemyHit);

	return true;
}
#pragma endregion