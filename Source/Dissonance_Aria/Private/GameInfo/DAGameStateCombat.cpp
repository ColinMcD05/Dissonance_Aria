


#include "GameInfo/DAGameStateCombat.h"

void ADAGameStateCombat::BeginPlay()
{
	Super::BeginPlay();
	OnEnemyDeath.AddDynamic(this, &ADAGameStateCombat::EnemyDead);
}

void ADAGameStateCombat::AddEnemies_Implementation(AActor* newEnemy)
{
	enemies.Add(newEnemy);
	IncreaseTotalExp(newEnemy);

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
		OnCombatEnd.Broadcast(totalEnemyExp);
	}
}

AActor* ADAGameStateCombat::GetNextEnemy(int& currentIndex)
{
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
	return enemies[currentIndex];
}

AActor* ADAGameStateCombat::GetPreviousEnemy(int& currentIndex)
{
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

	return enemies[currentIndex];
}