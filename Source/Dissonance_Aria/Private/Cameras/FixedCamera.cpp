


#include "Cameras/FixedCamera.h"
#include "BasicFunctions/GetGameInfo.h"
#include "GameInfo/DAGameInstance.h"

// Sets default values
AFixedCamera::AFixedCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	RootComponent = camera;
}

// Called when the game starts or when spawned
void AFixedCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFixedCamera::SetName()
{
	GameInfoUtilities::GetDAGameInstance(this)->SetLastCamera(GetName());
}