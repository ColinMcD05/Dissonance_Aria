


#include "Player/PlayerCharacterCombat.h"
#include "GameInfo/DAGameInstance.h"

// Sets default values
APlayerCharacterCombat::APlayerCharacterCombat()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup Components
	//Setup Combat System
	combatSystem = CreateDefaultSubobject<UCombatSystemComponent>(TEXT("CombatSystem"));

	//Setup Weapons and weapons system
	weaponsSystem = CreateDefaultSubobject<UWeaponsSystemComponent>(TEXT("WeaponsSystem"));
}

// Called when the game starts or when spawned
void APlayerCharacterCombat::BeginPlay()
{
	Super::BeginPlay();
	
	inventory = GameInfoUtilities::GetDAGameInstance(this)->GetInventory();

	if (weaponsSystem && inventory)
	{
		weaponsSystem->SpawnWeapons(inventory, this);
	}
}

// Called every frame
void APlayerCharacterCombat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacterCombat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

