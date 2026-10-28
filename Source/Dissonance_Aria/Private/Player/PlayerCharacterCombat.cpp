


#include "Player/PlayerCharacterCombat.h"
#include "GameInfo/DAGameInstance.h"
#include "Input/InputActions/CombatActionBase.h"
#include "InputAction.h"
#include "GameInfo/DAGameStateCombat.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameInfo/DAGameModeBase.h"
#include "CombatSystem/DodgeComponent.h"
#include "Music/MusicManagerCombat.h"

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

	//Setup Dodge Component
	dodgeComponent = CreateDefaultSubobject<UDodgeComponent>(TEXT("DodgeComponent"));
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

		weapon = weaponsSystem->GetCurrentHeldWeapon();
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
		gameState->OnCombatEnd.AddDynamic(this, &APlayerCharacterCombat::CombatEnded);
	}

	OnPlayerSetUpDone.Broadcast();

	CameraEnemySearch(nullptr);
	FocusOnEnemy();

	musicManager = Cast<AMusicManagerCombat>(UGameplayStatics::GetActorOfClass(this, AMusicManagerCombat::StaticClass()));

	if (IsValid(musicManager))
	{
		GameInfoUtilities::PrintToScreen("Added");

		musicManager->OnBeat.AddDynamic(this, &APlayerCharacterCombat::CanPauseGame);
	}

	if (IsValid(dodgeComponent))
	{
		dodgeComponent->OnDodge.AddDynamic(this, &APlayerCharacterCombat::ListenToDodge);
	}
}

#pragma region Inputs
// Called to bind functionality to input
void APlayerCharacterCombat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		inputComponent->BindAction(normalAttack, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadNormalAttack);
		inputComponent->BindAction(chargeAttack, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadChargeAttack);
		inputComponent->BindAction(chargeAttack, ETriggerEvent::Completed, this, &APlayerCharacterCombat::StopChargedAttack);
		inputComponent->BindAction(chargeAttack, ETriggerEvent::Canceled, this, &APlayerCharacterCombat::StopChargedAttack);
		inputComponent->BindAction(sideStep, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadSidestep);
		//inputComponent->BindAction(swapWeapon, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadSwapWeapon);
		inputComponent->BindAction(swapTuning, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadSwapTuning);
		inputComponent->BindAction(changeLockon, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadChangeLockOn);
		PlayerInputComponent->BindKey(EKeys::AnyKey, IE_Pressed, this, &APlayerCharacterCombat::ReadAnyKey);
	}
}

//Read the light attack input
void APlayerCharacterCombat::ReadNormalAttack()
{
	if (combatSystem)
	{
		if (combatSystem->ReadInput())
		{
			weaponsSystem->GetCurrentHeldWeapon()->StartSphereTrace();
			StartSnapToEnemy();

			if (canMove)
				canMove = false;
		}
	}
}

//Read the heavy attack input
void APlayerCharacterCombat::ReadChargeAttack()
{
	if (isCharging)
		return;

	if (combatSystem)
	{
		if (combatSystem->StartCharge())
		{
			canMove = false;

		}
	}
}

//Read the side step input
void APlayerCharacterCombat::ReadSidestep()
{
	if (!IsValid(dodgeComponent))
		return;

	dodgeComponent->StartDodge(direction);

	switch (direction)
	{
		case E_Directions::Backward:
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Backward"));
			break;
		case E_Directions::Left:
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Left"));
			break;
		case E_Directions::Right:
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Right"));
			break;
		case E_Directions::Forward:
		default:
			break;
	}
}

void APlayerCharacterCombat::ReadSwapTuning(const FInputActionValue& value)
{
	float inputValue = value.Get<float>() - 2;

	float tuning = weaponsSystem->CanTune(inputValue);

	if (tuning > -1)
	{
		StartTuning(inputValue);
	}
	else if (tuning == -1)
	{
		weaponsSystem->SwapTunings(-1);
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

void APlayerCharacterCombat::ReadAnyKey_Implementation(FKey pressedKey)
{
	ADAGameModeBase* gameModeBase = GameInfoUtilities::GetDAGameMode<ADAGameModeBase>(this);

	if (combatEnd && gameModeBase)
	{
		gameModeBase->TransitionToLevel("None");
	}

	if (KeyboardOrGamepad(pressedKey) != controllerType)
	{
		controllerType = KeyboardOrGamepad(pressedKey);
		OnSwapController.Broadcast(controllerType);
	}
}

void APlayerCharacterCombat::PauseGame_Implementation()
{
	if (canPause)
	{
		Super::PauseGame_Implementation();
	}
}

void APlayerCharacterCombat::CanPauseGame()
{
	GameInfoUtilities::PrintToScreen("Beat");

	canPause = true;

	musicManager->OnBeat.Remove(this, "CanPauseGame");
}
#pragma endregion

#pragma region Movement
void APlayerCharacterCombat::Move_Implementation(const FInputActionValue& value)
{
	if (canMove)
	{
		Super::Move_Implementation(value);
	}

	direction = CalculateDirection(value.Get<FVector>());
}

void APlayerCharacterCombat::StartSnapToEnemy()
{
	if (IsValid(lockedOnEnemy))
	{
		endPoint = lockedOnEnemy->GetActorLocation() - GetActorLocation();
		endPoint.Normalize();

		FVector endPosition = GetActorLocation() + (endPoint * snapDistance);

		FHitResult result;

		UKismetSystemLibrary::LineTraceSingle(this, GetActorLocation(), endPosition,
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4), false, {weapon}, EDrawDebugTrace::Persistent, result, true);

		if (result.GetActor() == lockedOnEnemy)
		{
			endPoint = GetActorLocation() + endPoint * (result.Distance - paddingDistance);
			SnapToEnemy();
		}
	}
}

void APlayerCharacterCombat::SnapToEnemy()
{
	if(GetActorLocation().Equals(endPoint, 0.1f))
	{
		SetActorLocation(endPoint);
		return;
	}
	else
	{
		FVector newPosition = FMath::VInterpConstantTo(GetActorLocation(), endPoint, GetWorld()->DeltaTimeSeconds, snapSpeed);

		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString::SanitizeFloat(newPosition.X).Append(FString::SanitizeFloat(newPosition.Y)).Append(FString::SanitizeFloat(newPosition.Z)));

		SetActorLocation(newPosition);

		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &APlayerCharacterCombat::SnapToEnemy);
	}
}

E_Directions APlayerCharacterCombat::CalculateDirection(FVector vector2)
{
	vector2.Normalize();

	//Calculate angle
	float angle = FMath::RadiansToDegrees(FMath::Atan2(vector2.Y, vector2.X));
	angle += 270;

	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString::SanitizeFloat(angle));

	//Return direction based on angle
	if (angle == 0 || (angle <= 240 && angle >= 120))
		return E_Directions::Backward;

	if (angle > 240 && angle < 360)
		return E_Directions::Right;

	if (angle > 0 && angle < 120)
		return E_Directions::Left;

	else return E_Directions::Backward;
}
#pragma endregion

E_ControllerType APlayerCharacterCombat::KeyboardOrGamepad(FKey pressedKey)
{
	if (pressedKey.IsGamepadKey())
	{
		return E_ControllerType::Controller;
	}
	if (pressedKey.IsTouch())
	{
		return E_ControllerType::Other;
	}
	else
	{
		return E_ControllerType::Keyboard;
	}
}

void APlayerCharacterCombat::ListenToDodge(E_Directions moveDirection)
{
	if (moveDirection == E_Directions::None)
	{
		canMove = true;
	}
	else canMove = false;
}

#pragma region Tuning
void APlayerCharacterCombat::ReadSuccess(bool successful, int tuning)
{
	if (successful && weaponsSystem)
	{
		weaponsSystem->SwapTunings(tuning);
		Execute_Heal(this, .15f, this);
	}
}
#pragma endregion

#pragma region Camera
//Relook for a valid enemy when one dies
void APlayerCharacterCombat::CameraEnemySearch(AActor* newEnemy)
{
	if (gameState)
	{
		if (lockedOnEnemy == newEnemy)
		{
			lockedOnEnemy = gameState->GetNextEnemy(enemyIndex);
		}
	}
}

void APlayerCharacterCombat::UpdateInventoryWeaponInfo(FS_WeaponInfo updatedInfo, int maxLevel)
{
	if (inventory)
	{
		inventory->UpdateWeaponInfo(updatedInfo, maxLevel);
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
void APlayerCharacterCombat::StartChargedAttack_Implementation()
{

}

void APlayerCharacterCombat::StopChargedAttack_Implementation()
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

void APlayerCharacterCombat::Heal_Implementation(float healAmount, AActor* healer)
{
	if (damageSystem)
	{
		damageSystem->HandleIncomingHeal(healAmount, healer);
	}
}

bool APlayerCharacterCombat::TakeDamage_Implementation(FS_DamageInfo damageInfo)
{
	int num = 0;
	if (damageSystem)
	{
		if (damageSystem->HandleIncomingDamage(damageInfo, num))
		{
			if (damageSystem->GetIsDead())
			{
				Execute_HandleDeath(this, damageInfo.damageCauser);
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

void APlayerCharacterCombat::CombatEnded(int exp)
{
	combatEnd = true;
}

#pragma region Old
/*

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
				didSwap = weaponsSystem->SwapWeapons(weapon1, 1);
				break;
			case 2:
				didSwap = weaponsSystem->SwapWeapons(weapon2, 2);
				break;
		}
	}

	if (didSwap && damageSystem)
	{
		damageSystem->SetMaxHealthAndCurrent(weaponsSystem->GetCurrentHeldWeapon()->GetMaxHealth());
	}
}
*/
#pragma endregion