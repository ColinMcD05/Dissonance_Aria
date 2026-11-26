


#include "Level/FogMinigame.h"
#include "Components/BoxComponent.h"
#include "Player/PlayerCharacterExploration.h"


// Sets default values
AFogMinigame::AFogMinigame()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	fogMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FogMesh"));
	fogMesh->SetupAttachment(root);

	blockingVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockingVolume"));
	blockingVolume->SetupAttachment(root);

	triggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	triggerVolume->SetCollisionProfileName("OverlapAllDynamic");
	triggerVolume->SetupAttachment(root);
}

// Called when the game starts or when spawned
void AFogMinigame::BeginPlay()
{
	Super::BeginPlay();
	
	triggerVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &AFogMinigame::TriggerOverlap);
	triggerVolume->OnComponentEndOverlap.AddUniqueDynamic(this, &AFogMinigame::TriggerEndOverlap);
}

void AFogMinigame::TriggerOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	APlayerCharacterExploration* characterExploration = Cast<APlayerCharacterExploration>(otherActor);

	if (IsValid(characterExploration))
	{
		characterExploration->SetCurrentMinigame(this);
	}
}

void AFogMinigame::TriggerEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	APlayerCharacterExploration* characterExploration = Cast<APlayerCharacterExploration>(otherActor);

	if (IsValid(characterExploration))
	{
		characterExploration->SetCurrentMinigame(nullptr);
	}
}

void AFogMinigame::RemoveFog()
{
	Destroy();
}