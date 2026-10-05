

#include "Weapons/WeaponActor.h"
#include "Player/PlayerCharacterCombat.h"
#include "GameInfo/DAGameStateCombat.h"
#include "BasicFunctions/GetGameInfo.h"

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
	SetActorHiddenInGame(false);
	activated = true;
	LowerTolerance();
	currentTuning = 0;
	ChangeWeaponStats();
	if (!raisingTolerance)
	{
		RaiseTuningTolerance();
	}
}

void AWeaponActor::Deactivate()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	activated = false;
	GetWorld()->GetTimerManager().SetTimer(raiserTimer, this, &AWeaponActor::RaiseTolerance, 5, false);
	RaiseTolerance();
	currentTuning = 0;
	if (!raisingTolerance)
	{
		RaiseTuningTolerance();
	}
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

float AWeaponActor::PerformLightAttack_Implementation(const TArray<E_CombatActionType>& previousActions)
{
	int animationPosition = CalculateAnimationPosition(previousActions, E_CombatActionType::LightAttack);
	//Animation logic will go here, but I need animations first
	return 1.5;
}

float AWeaponActor::PerformHeavyAttack_Implementation(const TArray<E_CombatActionType>& previousActions)
{
	int animationPosition = CalculateAnimationPosition(previousActions, E_CombatActionType::HeavyAttack);
	//Animation logic will go here, but I need animations first
	return 1.5;
}


void AWeaponActor::StartChargeAttack_Implementation()
{
	originalLocation = weaponMesh->GetRelativeLocation();
	speed = 200 / 0.8;
	ChargeAttack();
}

void AWeaponActor::ChargeAttack_Implementation()
{
	if (originalLocation.Y - 100 <= weaponMesh->GetRelativeLocation().Y)
	{
		FVector addedVector = FVector(0, -speed * GetWorld()->DeltaTimeSeconds, 0);
		weaponMesh->AddRelativeLocation(addedVector);
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AWeaponActor::ChargeAttack);
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Yes");
		}
		playerOwner->ChargeReady();
	}
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
	if (currentTuning < statsTable.Num() && statsTable[currentTuning])
	{
		FS_Stats* newStats = statsTable[currentTuning]->FindRow<FS_Stats>(FName(*FString::FromInt(weaponInfo.level)), "", true);
		if (newStats)
		{
			stats = *newStats;
		}
		if (newStats && depleted)
		{
			stats.damage *= depletionAmount;
			stats.sidestepDistance *= depletionAmount;
			stats.speed *= depletionAmount;
		}
	}
}

void AWeaponActor::CombatEnd(int gainedExp)
{
	weaponInfo.exp.currentExperience += gainedExp;
	playerOwner->UpdateInventoryWeaponInfo(weaponInfo);
}

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
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Depleted"));
			}
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
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Regained"));
			}
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
		if(i == currentTuning)
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

	if (weaponInfo.tunings[tuning - 1].toleranceMeter < 1)
	{
		return -1;
	}

	if (tuning == currentTuning)
	{
		return 0;
	}
	return 1;
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

	return weaponInfo.tunings[currentTuning].toleranceMeter;
}

int AWeaponActor::GetCurrentTuning()
{
	return currentTuning;
}