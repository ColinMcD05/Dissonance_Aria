


#include "GameInfo/DAGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void ADAGameModeBase::TransitionToLevel_Implementation(FName levelName)
{
	UGameplayStatics::OpenLevel(GetWorld(), levelName);
}

void ADAGameModeBase::LevelOpened_Implementation()
{

}