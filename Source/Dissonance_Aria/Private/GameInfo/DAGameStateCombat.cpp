


#include "GameInfo/DAGameStateCombat.h"
#include "Enemies/LockOnInterface.h"
#include "BasicFunctions/GetGameInfo.h"

void ADAGameStateCombat::BeginPlay()
{
	Super::BeginPlay();
	OnEnemyDeath.AddUniqueDynamic(this, &ADAGameStateCombat::EnemyDead);
}

void ADAGameStateCombat::CombatStarted()
{
	finished = false;
}

void ADAGameStateCombat::AddEnemies_Implementation(AActor* newEnemy)
{
	enemies.Add(newEnemy);
	IncreaseTotalExpAndGold(newEnemy);

	if (enemies.Num() == 1)
	{
		OnEnemySpawned.Broadcast();
	}
}

void ADAGameStateCombat::EnemyDead_Implementation(AActor* newEnemy)
{
	enemies.Remove(newEnemy);

	if (enemies.Num() <= 0 && !finished)
	{
		finished = true;
		OnCombatEnd.Broadcast(totalEnemyExp, totalEnemyGold);
	}
}

AActor* ADAGameStateCombat::GetNextEnemy(int& currentIndex)
{
	int oldIndex = currentIndex;

	if (enemies.Num() == 0)
	{
		currentIndex = -1;
		return nullptr;
	}

	currentIndex++;
	if (currentIndex >= enemies.Num())
	{
		currentIndex = 0;
	}

	if (oldIndex >= 0 && enemies.Num() > oldIndex)
	{
		UnsendLockedOn(enemies[oldIndex]);
	}

	if (currentIndex >= 0 && enemies.Num() > 0)
	{
		SendLockedOn(enemies[currentIndex]);
	}

	return enemies[currentIndex];
}

AActor* ADAGameStateCombat::GetPreviousEnemy(int& currentIndex)
{
	if (currentIndex >= 0 && enemies.Num() > 0)
	{
		UnsendLockedOn(enemies[currentIndex]);
	}

	if (enemies.Num() == 0)
	{
		currentIndex = -1;
		return nullptr;
	}

	currentIndex--;
	if (currentIndex < 0)
	{
		currentIndex = enemies.Num() - 1;
	}

	if (currentIndex >= 0 && enemies.Num() > 0)
	{
		SendLockedOn(enemies[currentIndex]);
	}

	return enemies[currentIndex];
}

AActor* ADAGameStateCombat::GetClosestEnemy(AActor* player, int& currentIndex)
{
	float closest = 0;
	AActor* closestEnemy = nullptr;

	for (int i = 0; i < enemies.Num(); i++)
	{
		float newDistance = player->GetDistanceTo(enemies[i]);

		if (closestEnemy == nullptr || newDistance < closest)
		{
			closest = newDistance;
			closestEnemy = enemies[i];
			currentIndex = i;
		}
	}

	if(IsValid(closestEnemy))
		SendLockedOn(closestEnemy);

	return closestEnemy;
}

void ADAGameStateCombat::SendLockedOn(AActor* newEnemy)
{
	if (!IsValid(newEnemy))
		return;

	if (newEnemy->GetClass()->ImplementsInterface(ULockOnInterface::StaticClass()))
	{
		ILockOnInterface::Execute_LockedOn(newEnemy);
	}
}

void ADAGameStateCombat::UnsendLockedOn(AActor* newEnemy)
{
	if (!IsValid(newEnemy))
		return;

	if (newEnemy->GetClass()->ImplementsInterface(ULockOnInterface::StaticClass()))
	{
		ILockOnInterface::Execute_LockedOff(newEnemy);
	}
}