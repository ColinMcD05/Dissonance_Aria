


#include "GameInfo/DAGameStateCombat.h"

void ADAGameStateCombat::AddEnemies_Implementation(AActor* newEnemy)
{
	enemies.Add(newEnemy);
	IncreaseTotalExp(newEnemy);
}

AActor* ADAGameStateCombat::GetNextEnemy(int& currentIndex)
{
	currentIndex++;
	if (enemies.Num() == 0)
	{
		currentIndex = -1;
		return nullptr;
	}
	if (currentIndex >= enemies.Num())
	{
		currentIndex = 0;
	}
	return enemies[currentIndex];
}

AActor* ADAGameStateCombat::GetPreviousEnemy(int& currentIndex)
{
	currentIndex--;
	if (currentIndex < 0)
	{
		currentIndex = enemies.Num() - 1;
	}

	return enemies[currentIndex];
}