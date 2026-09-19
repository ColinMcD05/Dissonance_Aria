


#include "Player/PlayerCharacterCombat.h"

// Sets default values
APlayerCharacterCombat::APlayerCharacterCombat()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacterCombat::BeginPlay()
{
	Super::BeginPlay();
	
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

