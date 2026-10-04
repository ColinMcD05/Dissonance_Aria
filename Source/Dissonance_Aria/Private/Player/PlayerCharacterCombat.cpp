


#include "Player/PlayerCharacterCombat.h"
#include "GameInfo/DAGameInstance.h"
#include "Input/InputActions/CombatActionBase.h"
#include "InputAction.h"
#include "GameInfo/DAGameStateCombat.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

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

	weaponSpot = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponsSpot"));
	weaponSpot->SetupAttachment(GetMesh());

	//Setup Damage System
	damageSystem = CreateDefaultSubobject <UDamageSystemComponent>(TEXT("DamageSystem"));

	//Setup camera
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	//SpringArm setup
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	springArm->SetupAttachment(RootComponent);
	springArm->TargetArmLength = distanceFromPlayer;
	springArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	//Camera lag. Comment out if unneeded
	if (cameraMoveLag)
	{
		springArm->bEnableCameraLag = true;
		springArm->CameraLagSpeed = 2;
		springArm->CameraLagMaxDistance = 1.5f;
	}

	if (cameraRotationLag)
	{
		springArm->bEnableCameraLag = true;
		springArm->CameraRotationLagSpeed = 4;
		springArm->CameraLagMaxTimeStep = 1;
	}

	camera->SetupAttachment(springArm, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void APlayerCharacterCombat::BeginPlay()
{
	Super::BeginPlay();
	
	//Gets the inventory component
	inventory = GameInfoUtilities::GetDAGameInstance(this)->GetInventory();

	//Spawns weapons in once in
	if (weaponsSystem && inventory)
	{
		weaponsSystem->SpawnWeapons(inventory, this);

		weapon1 = weaponsSystem->GetCurrentHeldWeapon();
		weapon2 = weaponsSystem->GetStoredWeapon();
	}

	if (damageSystem)
	{
		damageSystem->SetMaxHealthAndCurrent(weaponsSystem->GetCurrentHeldWeapon()->GetMaxHealth());
	}

	//Get game state
	gameState = GameInfoUtilities::GetDAGameState<ADAGameStateCombat>(this);
	if (gameState)
	{
		gameState->OnEnemyDeath.AddDynamic(this, &APlayerCharacterCombat::CameraEnemySearch);
	}

	CameraEnemySearch(nullptr);
	FocusOnEnemy();
}

// Called every frame
void APlayerCharacterCombat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region Inputs
// Called to bind functionality to input
void APlayerCharacterCombat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		inputComponent->BindAction(lightAttack, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadLightAttack);
		inputComponent->BindAction(heavyAttack, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadHeavyAttack);
		inputComponent->BindAction(sideStep, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadSidestep);
		inputComponent->BindAction(swapWeapon, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadSwapWeapon);
		inputComponent->BindAction(changeLockon, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadChangeLockOn);
	}
}

//Read the light attack input
void APlayerCharacterCombat::ReadLightAttack()
{
	if (combatSystem)
	{
		combatSystem->AddToCombatQueue(E_CombatActionType::LightAttack);
	}
}

//Read the heavy attack input
void APlayerCharacterCombat::ReadHeavyAttack()
{
	if (combatSystem)
	{
		if (combatSystem->GetPreviousActionsAmount() > 0)
		{
			combatSystem->AddToCombatQueue(E_CombatActionType::HeavyAttack);
		}
		else
		{
			StartHeavyAttack();
		}
	}
}

//Read the side step input
void APlayerCharacterCombat::ReadSidestep()
{

}

//Read Swap Input
void APlayerCharacterCombat::ReadSwapWeapon(const FInputActionValue& value)
{
	bool didSwap = false;
	int swapValue = value.Get<float>();
	if (weaponsSystem)
	{
		switch (swapValue)
		{
		case 1:
			weaponsSystem->SwapWeapons(weapon1, 1);
			break;
		case 2:
			weaponsSystem->SwapWeapons(weapon2, 2);
			break;
		}
	}

	if (didSwap && damageSystem)
	{
		damageSystem->SetMaxHealthAndCurrent(weaponsSystem->GetCurrentHeldWeapon()->GetMaxHealth());
	}
}

//Read the change lock on input
void APlayerCharacterCombat::ReadChangeLockOn(const FInputActionValue& value)
{
	float changeTo = value.Get<float>();

	if (gameState)
	{
		if (changeTo > 0)
		{
			lockedOnEnemy = gameState->GetNextEnemy(enemyIndex);
		}
		else
		{
			lockedOnEnemy = gameState->GetPreviousEnemy(enemyIndex);
		}
	}
}
#pragma endregion

#pragma region Camera
//Relook for a valid enemy when one dies
void APlayerCharacterCombat::CameraEnemySearch(AActor* newEnemy)
{
	if (gameState)
	{
		if (!lockedOnEnemy)
		{
			lockedOnEnemy = gameState->GetNextEnemy(enemyIndex);
		}
	}
}

void APlayerCharacterCombat::UpdateInventoryWeaponInfo(FS_WeaponInfo updatedInfo)
{
	if (inventory)
	{
		inventory->UpdateWeaponInfo(updatedInfo);
	}
}

void APlayerCharacterCombat::FocusOnEnemy()
{
	if (lockedOnEnemy && enemyIndex > -1)
	{
		RotatePlayer();
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &APlayerCharacterCombat::FocusOnEnemy);
	}
}

void APlayerCharacterCombat::RotatePlayer()
{
	if (camera)
	{
		//Get angle
		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(camera->GetComponentLocation(), lockedOnEnemy->GetActorLocation());

		FRotator newRotation = FMath::RInterpTo(GetActorRotation(), rotator, GetWorld()->DeltaTimeSeconds, rotateSpeed);

		Controller->SetControlRotation(FRotator(GetActorRotation().Pitch, newRotation.Yaw, GetActorRotation().Roll));
	}
}
#pragma endregion

//Logic specifically for heavy attacks
#pragma region HeavyAttack
void APlayerCharacterCombat::StartHeavyAttack_Implementation()
{

}

#pragma endregion

//Implementation of Attack interface
#pragma region AttacksInterface
float APlayerCharacterCombat::LightAttack_Implementation(TArray<E_CombatActionType>& previousActions)
{
	return weaponsSystem->GetCurrentHeldWeapon()->PerformLightAttack(previousActions);
}

float APlayerCharacterCombat::HeavyAttack_Implementation(TArray<E_CombatActionType>& previousActions, bool charged)
{
	return weaponsSystem->GetCurrentHeldWeapon()->PerformHeavyAttack(previousActions);
}

float APlayerCharacterCombat::SpecialAttack_Implementation()
{
	return 0;
}
#pragma endregion

#pragma region DamageableInterface
float APlayerCharacterCombat::GetCurrentHealth_Implementation()
{
	if (damageSystem)
	{
		return damageSystem->GetCurrentHealth();
	}
	return 0;
}

float APlayerCharacterCombat::GetMaxHealth_Implementation()
{
	if (damageSystem)
	{
		return damageSystem->GetMaxHealth();
	}
	return 0;
}

bool APlayerCharacterCombat::GetIsDead_Implementation()
{
	if (damageSystem)
	{
		return damageSystem->GetIsDead();
	}
	return false;
}

void APlayerCharacterCombat::Heal_Implementation(float HealAmount, AActor* Healer)
{

}

bool APlayerCharacterCombat::TakeDamage_Implementation(FS_DamageInfo damageInfo)
{
	if (damageSystem)
	{
		if (damageSystem->HandleIncomingDamage(damageInfo))
		{
			if (damageSystem->GetIsDead())
			{
				HandleDeath(damageInfo.damageCauser);
			}
			return true;
		}
	}
	return false;
}

void APlayerCharacterCombat::HandleDeath_Implementation(AActor* killer)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			subsystem->ClearAllMappings();
		}
	}
	GetWorld()->GetTimerManager().SetTimer(resetLevelTimer, this, &APlayerCharacterCombat::ResetCurrentLevel, 3, false);
}
#pragma endregion

void APlayerCharacterCombat::ResetCurrentLevel()
{
	const FString CurrentMap = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentMap));
}