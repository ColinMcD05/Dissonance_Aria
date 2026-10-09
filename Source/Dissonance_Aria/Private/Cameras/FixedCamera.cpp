


#include "Cameras/FixedCamera.h"

// Sets default values
AFixedCamera::AFixedCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->Activate(false);
	
	RootComponent = camera;

}

// Called when the game starts or when spawned
void AFixedCamera::BeginPlay()
{
	Super::BeginPlay();
	
}