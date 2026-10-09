


#include "Cameras/CameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacterExploration.h"

// Sets default values
ACameraManager::ACameraManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraManager::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> foundCamera;
		
	UGameplayStatics::GetAllActorsOfClass(this, AFixedCamera::StaticClass(), foundCamera);

	//Find all fixed cameras
	for (AActor* cam : foundCamera)
	{
		if (AFixedCamera* fixCamera = Cast<AFixedCamera>(cam))
		{
			cameras.Add(fixCamera);
		}
	}

	OnCameraChange.AddDynamic(this, &ACameraManager::SwitchCamera);
}

void ACameraManager::SwitchCamera(AFixedCamera* newCamera)
{
	//Set current camera
	currentCamera = newCamera;

	if (APlayerCharacterExploration* explorer = Cast<APlayerCharacterExploration>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		explorer->SetCurrentCamera(newCamera->GetCamera());
	}
}