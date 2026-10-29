


#include "GameInfo/DAGameStateCombat.h"
#include "Enemies/LockOnInterface.h"

void ADAGameStateCombat::BeginPlay()
{
	Super::BeginPlay();
	OnEnemyDeath.AddDynamic(this, &ADAGameStateCombat::EnemyDead);
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