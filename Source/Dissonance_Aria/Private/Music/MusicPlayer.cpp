


#include "Music/MusicPlayer.h"

// Sets default values for this component's properties
UMusicPlayer::UMusicPlayer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMusicPlayer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMusicPlayer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMusicPlayer::ChangeBPM(int newBPM)
{
	bpm = newBPM;
	OnBPMChanged.Broadcast(bpm);
}

//float UMusicPlayer