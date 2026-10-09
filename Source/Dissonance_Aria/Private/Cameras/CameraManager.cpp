


#include "Cameras/CameraManager.h"
#include "Kismet/GameplayStatics.h"

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

	for (AActor* cam : foundCamera)
	{
		if (AFixedCamera* fixCamera = Cast<AFixedCamera>(cam))
		{
			
		}
	}
}