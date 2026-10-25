


#include "Music/MusicManagerCombat.h"
#include "GameInfo/DAGameStateCombat.h"
#include "BasicFunctions/GetGameInfo.h"
#include "Player/PlayerCharacterCombat.h"
#include "Kismet/GameplayStatics.h"
#include "FMODBlueprintStatics.h"

// Sets default values
AMusicManagerCombat::AMusicManagerCombat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup audio component
	audioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("FMODAudio"));
	audioComponent->bEnableTimelineCallbacks = true;
}

#pragma region General
void AMusicManagerCombat::BeginPlay()
{
	Super::BeginPlay();
	audioComponent->OnTimelineBeat.AddDynamic(this, &AMusicManagerCombat::OnDownBeat);
	BindEvents();
}

void AMusicManagerCombat::BindEvents()
{
	//Get game state
	ADAGameStateCombat* combat = GameInfoUtilities::GetDAGameState<ADAGameStateCombat>(this);

	//Setup game state events
	if (combat)
	{
		combat->OnCombatStart.AddDynamic(this, &AMusicManagerCombat::StartMusic);
		combat->OnCombatEnd.AddDynamic(this, &AMusicManagerCombat::StopMusic);
	}

	//Get player character
	APlayerCharacterCombat* character = Cast<APlayerCharacterCombat>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (character)
	{
		character->GetWeaponsSystem()->OnChangeMusic.AddDynamic(this, &AMusicManagerCombat::ChangeChannel);
	}
}
#pragma endregion

#pragma region Music
//Starts music
void AMusicManagerCombat::StartMusic()
{
	if (audioComponent)
		audioComponent->Play();
}

//Stops music
void AMusicManagerCombat::StopMusic(int exp)
{
	if (audioComponent)
		audioComponent->Stop();
}

void AMusicManagerCombat::ChangeChannel(int newChannel)
{
	//New channels
	currentChannel = newChannel;
	GameInfoUtilities::PrintToScreen(FString::SanitizeFloat(newChannel));
	UFMODBlueprintStatics::SetGlobalParameterByName("EquippedWeapon", newChannel);
}
#pragma endregion

#pragma region Position
//Get the current time of the song. -1 if not playing or audio component does not exist
int AMusicManagerCombat::GetCurrentSongTime()
{
	//Check if audio is playing
	if (audioComponent == nullptr && !audioComponent->IsPlaying())
		return -1;

	//Return timeline position
	return audioComponent->GetTimelinePosition();
}

//Get the position of the beat
float AMusicManagerCombat::GetPositionOfBeat()
{
	//Get and validate the song time
	float songTime = GetCurrentSongTime();

	if (songTime == -1)
		return -1;

	//Position based on changed bpm time
	songTime = songTime - timeConsideredStart;

	//Convert to seconds
	songTime /= 1000;

	//Return position of song
	return songTime / secondsPerBeat;
}

//Get how close the the beat at current time
float AMusicManagerCombat::GetHowCloseToBeat()
{
	//Get and validate position of beat
	float position = GetPositionOfBeat();

	if (position == -1)
		return -1;

	return position - FMath::Floor(position);
}

int AMusicManagerCombat::GetFocusedBeat()
{
	return currentBeat + FMath::RoundToInt(GetHowCloseToBeat());
}

int AMusicManagerCombat::GetBeatInTimeSig()
{
	return (FMath::RoundToInt(GetPositionOfBeat()) % 4) + 1;
}
#pragma endregion

#pragma region OnBeat
void AMusicManagerCombat::OnDownBeat(int bar, int beat, int position, float tempo, int upperTimeSig, int lowerTimeSig)
{

	currentBeat++;

	OnBeat.Broadcast();

	//Check if bpm changed
	if (bpm != tempo)
	{
		//Update information
		bpm = tempo;
		timeConsideredStart = position;
		secondsPerBeat = 60/bpm;

		//Broadcast new info
		OnBPMChange.Broadcast(bpm, secondsPerBeat);
	}
}
#pragma endregion