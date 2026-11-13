


#include "Chest/Chest.h"
#include "Player/PlayerCharacterExploration.h"
#include "GameInfo/DAGameInstance.h"
#include "Components/BoxComponent.h"
#include "BasicFunctions/GetGameInfo.h"

// Sets default values
AChest::AChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create mesh
	chestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestMesh"));
	RootComponent = chestMesh;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	boxComponent->SetCollisionProfileName("OverlapAllDynamic");
	boxComponent->SetupAttachment(chestMesh);
}

// Called when the game starts or when spawned
void AChest::BeginPlay()
{
	Super::BeginPlay();

	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AChest::StartedOverlap);
	boxComponent->OnComponentEndOverlap.AddDynamic(this, &AChest::EndOverlap);
}

void AChest::SetUp(bool newPickedUp)
{
	pickedUp = newPickedUp;
}

void AChest::Interact_Implementation(APlayerCharacterExploration* player)
{
	if (pickedUp || !IsValid(player) || !IsValid(equipment))
		return;

	pickedUp = true;
	GameInfoUtilities::GetDAGameInstance(this)->GetInventory()->GainEquipment(equipment);
}

void AChest::StartedOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (APlayerCharacterExploration* player = Cast<APlayerCharacterExploration>(otherActor))
	{
		player->SetInteractingObject(this);
	}
}

void AChest::EndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (APlayerCharacterExploration* player = Cast<APlayerCharacterExploration>(otherActor))
	{
		player->SetInteractingObject(this, true);
	}
}