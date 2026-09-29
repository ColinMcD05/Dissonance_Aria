


#include "Player/PlayerCharacterBase.h"
#include "Player/PlayerControllerBase.h"
#include "EnhancedInputComponent.h"

// Sets default values
APlayerCharacterBase::APlayerCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		inputComponent->BindAction(move, ETriggerEvent::Triggered, this, &APlayerCharacterBase::Move);
		inputComponent->BindAction(jumpAction, ETriggerEvent::Started, this, &APlayerCharacterBase::PlayerJump);
		inputComponent->BindAction(jumpAction, ETriggerEvent::Completed, this, &APlayerCharacterBase::StopPlayerJump);
		inputComponent->BindAction(pause, ETriggerEvent::Triggered, this, &APlayerCharacterBase::PauseGame);

		if (useFreeCamera)
		{
			inputComponent->BindAction(look, ETriggerEvent::Triggered, this, &APlayerCharacterBase::Look);
		}
	}
}

void APlayerCharacterBase::Move_Implementation(const FInputActionValue& value)
{
	FVector2D moveVector = value.Get<FVector2D>();
	
	AddMovementInput(GetActorRightVector(), moveVector.X);
	AddMovementInput(GetActorForwardVector(), moveVector.Y);
}

void APlayerCharacterBase::PlayerJump_Implementation()
{
	Jump();
}

void APlayerCharacterBase::StopPlayerJump_Implementation()
{
	StopJumping();
}

void APlayerCharacterBase::Look_Implementation(const FInputActionValue& value)
{
	FVector2D lookVector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(lookVector.X);
		AddControllerYawInput(lookVector.Y);
	}
}

void APlayerCharacterBase::PauseGame_Implementation()
{
	Cast<APlayerControllerBase>(GetController())->PausedGame();
}