

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FogMinigame.generated.h"

class USceneComponent;
class UBoxComponent;
class UStaticMeshComponent;
class UPDA_SongInfo;

UCLASS()
class DISSONANCE_ARIA_API AFogMinigame : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* fogMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collison", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* blockingVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collison", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* triggerVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Song", meta = (AllowPrivateAccess = "true"))
	UPDA_SongInfo* attachedSong;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	AFogMinigame();

	void RemoveFog();

#pragma region Overlap
	UFUNCTION()
	void TriggerOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
		UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void TriggerEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
		UPrimitiveComponent* otherComp, int32 otherBodyIndex);
#pragma endregion

#pragma region Song
	UPDA_SongInfo* GetAttachedSong() { return attachedSong; }
#pragma endregion
};
