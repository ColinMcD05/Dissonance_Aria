

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMODAudioComponent.h"
#include "MusicManagerCombat.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBPMChange, float, newBPM, float, newSPB);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeat);

UCLASS()
class DISSONANCE_ARIA_API AMusicManagerCombat : public AActor
{
	GENERATED_BODY()

private:
#pragma region Music Info
	UPROPERTY(BlueprintReadOnly, Category = "MusicInfo", meta = (AllowPrivateAccess = "true"))
	float bpm = 0;

	UPROPERTY(BlueprintReadOnly, Category = "MusicInfo", meta = (AllowPrivateAccess = "true"))
	int currentBeat = 0;

	UPROPERTY(BlueprintReadOnly, Category = "MusicInfo", meta = (AllowPrivateAccess = "true"))
	float secondsPerBeat;

	//The spot of the song where bpm most recently changed
	UPROPERTY(BlueprintReadOnly, Category = "MusicInfo", meta = (AllowPrivateAccess = "true"))
	int timeConsideredStart;

	UPROPERTY(BlueprintReadOnly, Category = "MusicInfo", meta = (AllowPrivateAccess = "true"))
	int currentChannel;
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UFMODAudioComponent* audioComponent;
#pragma endregion

public:	
#pragma region General
	// Sets default values for this actor's properties
	AMusicManagerCombat();

	void BindEvents();
#pragma endregion

#pragma region Position
	//Get the current time of the song
	UFUNCTION(BlueprintCallable)
	int GetCurrentSongTime();

	//Get the position of the beat
	UFUNCTION(BlueprintCallable)
	float GetPositionOfBeat();

	//Get how close the the beat at current time
	UFUNCTION(BlueprintCallable)
	float GetHowCloseToBeat();

	UFUNCTION(BlueprintCallable)
	int GetFocusedBeat();
#pragma endregion

#pragma region Music
	//Start music
	UFUNCTION()
	void StartMusic();

	//Stop music
	UFUNCTION()
	void StopMusic(int exp);

	//Change the music channel
	UFUNCTION()
	void ChangeChannel(int newChannel);
#pragma region OnBeat
	//Runs on every beat to check and/or change
	UFUNCTION()
	void OnDownBeat(int bar, int beat, int position, float tempo, int upperTimeSig, int lowerTimeSig);
#pragma endregion

#pragma region Delegates
	//Delegate that fires once bpm changes
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnBPMChange OnBPMChange;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnBeat OnBeat;
#pragma endregion
};