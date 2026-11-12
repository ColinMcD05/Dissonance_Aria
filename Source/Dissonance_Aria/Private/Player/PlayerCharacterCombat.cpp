


#include "Player/PlayerCharacterCombat.h"
#include "Player/PlayerControllerBase.h"
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
#include "GameFramework/CharacterMovementComponent.h"
#include "Music/MusicManagerCombat.h"
#include "Inventory/PDA_Equipment.h"

#pragma region General Setup
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

	if (IsValid(dodgeComponent) && IsValid(combatSystem))
	{
		dodgeComponent->OnDodge.AddUniqueDynamic(this, &APlayerCharacterCombat::ListenToDodge);
		dodgeComponent->OnDodge.AddUniqueDynamic(combatSystem, &UCombatSystemComponent::SetIsDodging);
	}

	if (IsValid(combatSystem))
	{
		combatSystem->OnChargeSuccessChange.AddDynamic(this, &APlayerCharacterCombat::ChargeAttack);
	}

	if (IsValid(damageSystem))
	{
		damageSystem->OnReactionChanged.AddDynamic(this, &APlayerCharacterCombat::ReactToDamage);
	}

	//Get game state
	gameState = GameInfoUtilities::GetDAGameState<ADAGameStateCombat>(this);
	if (gameState)
	{
		gameState->OnEnemyDeath.AddUniqueDynamic(this, &APlayerCharacterCombat::CameraEnemySearch);
		gameState->OnCombatEnd.AddUniqueDynamic(this, &APlayerCharacterCombat::CombatEnded);
		gameState->OnCombatStart.AddUniqueDynamic(this, &APlayerCharacterCombat::CombatStarted);
	}
}
#pragma endregion

#pragma region Combat Setup
void APlayerCharacterCombat::UpdateHealth(int weaponHealth)
{
	if (!damageSystem) return;

	damageSystem->SetMaxHealthAndCurrent(weaponHealth + healthIncrease);
}

void APlayerCharacterCombat::UpdateInventoryWeaponInfo(FS_WeaponInfo updatedInfo, int maxLevel)
{
	if (inventory)
	{
		inventory->UpdateWeaponInfo(updatedInfo, maxLevel);
	}
}

void APlayerCharacterCombat::CombatStarted_Implementation()
{
	lockOn = false;

	canMove = true;

	GetEquipmentInfo();

	if (damageSystem)
	{
		damageSystem->SetInitialHealth(weaponsSystem->GetCurrentHeldWeapon()->GetMaxHealth());
	}

	OnPlayerSetUpDone.Broadcast();
}

void APlayerCharacterCombat::SetMusicManager()
{
	musicManager = Cast<AMusicManagerCombat>(UGameplayStatics::GetActorOfClass(this, AMusicManagerCombat::StaticClass()));

	if (combatSystem)
		combatSystem->SetUp(musicManager);

	if (dodgeComponent)
		dodgeComponent->SetUp(musicManager);

	if (IsValid(musicManager))
	{
		//GameInfoUtilities::PrintToScreen("Added");

		musicManager->OnBeat.AddUniqueDynamic(this, &APlayerCharacterCombat::CanPauseGame);
	}
}
#pragma endregion

#pragma region Inputs
// Called to bind functionality to input
void APlayerCharacterCombat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	useFreeCamera = true;

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		inputComponent->BindAction(GetMove(), ETriggerEvent::Canceled, this, &APlayerCharacterCombat::MovementStoped);
		inputComponent->BindAction(GetMove(), ETriggerEvent::Completed, this, &APlayerCharacterCombat::MovementStoped);
		inputComponent->BindAction(normalAttack, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadNormalAttack);
		inputComponent->BindAction(chargeAttack, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadChargeAttack);
		inputComponent->BindAction(chargeAttack, ETriggerEvent::Completed, this, &APlayerCharacterCombat::StopChargedAttack);
		inputComponent->BindAction(chargeAttack, ETriggerEvent::Canceled, this, &APlayerCharacterCombat::StopChargedAttack);
		inputComponent->BindAction(sideStep, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadSidestep);
		//inputComponent->BindAction(swapWeapon, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadSwapWeapon);
		inputComponent->BindAction(swapTuning, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadSwapTuning);
		inputComponent->BindAction(changeLockon, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadChangeLockOn);
		inputComponent->BindAction(changeCameraType, ETriggerEvent::Started, this, &APlayerCharacterCombat::ReadChangeCameraType);
		PlayerInputComponent->BindKey(EKeys::AnyKey, IE_Pressed, this, &APlayerCharacterCombat::ReadAnyKey);
	}
}

//Read the light attack input
void APlayerCharacterCombat::ReadNormalAttack()
{
	if (!canReadInput)
		return;

	//GameInfoUtilities::PrintToScreen("Attack");
	if (combatSystem)
	{
		if (combatSystem->ReadInput())
		{
			weaponsSystem->GetCurrentHeldWeapon()->StartSphereTrace();
			//StartSnapToEnemy();

			if (canMove)
				canMove = false;
		}
		else 
		{
			//GameInfoUtilities::PrintToScreen("Bad Player");
		}
	}
	else
	{
		//GameInfoUtilities::PrintToScreen("No Combat");
	}
}

//Read the heavy attack input
void APlayerCharacterCombat::ReadChargeAttack()
{
	if (!canReadInput)
		return;

	if (isCharging)
		return;

	if (combatSystem)
	{
		if (combatSystem->StartCharge())
		{
			canMove = false;
			isCharging = true;
		}
	}
}

//Read the side step input
void APlayerCharacterCombat::ReadSidestep()
{
	if (!canReadInput)
		return;

	if (!IsValid(dodgeComponent) || GetCharacterMovement()->IsFalling())
		return;

	dodgeComponent->StartDodge(direction);

	switch (direction)
	{
		case E_Directions::Backward:
			//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Backward"));
			break;
		case E_Directions::Left:
			//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Left"));
			break;
		case E_Directions::Right:
			//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Right"));
			break;
		case E_Directions::Forward:
		default:
			break;
	}
}

void APlayerCharacterCombat::ReadSwapTuning(const FInputActionValue& value)
{
	if (!canReadInput)
		return;

	float inputValue = value.Get<float>() - 2;

	TArray<E_QTEInput> qteInputs;

	float tuning = weaponsSystem->CanTune(inputValue, qteInputs);

	if (tuning > -1)
	{
		StartTuning(inputValue, qteInputs);
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
		gameModeBase->LoadExploration();
	}

	if (KeyboardOrGamepad(pressedKey) != controllerType)
	{
		controllerType = KeyboardOrGamepad(pressedKey);
		OnSwapController.Broadcast(controllerType);
	}
}

void APlayerCharacterCombat::ReadChangeCameraType()
{
	lockOn = !lockOn;

	if (lockOn)
	{
		CameraEnemySearch(nullptr);
		FocusOnEnemy();
	}
	else
	{
		if(IsValid(lockedOnEnemy))
			gameState->UnsendLockedOn(lockedOnEnemy);
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
	//GameInfoUtilities::PrintToScreen("Beat");

	canPause = true;

	musicManager->OnBeat.Remove(this, "CanPauseGame");
}

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
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel3), false, {weapon}, EDrawDebugTrace::Persistent, result, true);

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

	if (angle > 360)
	{
		angle -= 360;
	}

	//GameInfoUtilities::PrintToScreen(FString::SanitizeFloat(angle));

	//Return direction based on angle
	if (angle <= 15 || angle >= 345)
		return E_Directions::Forward;

	if (angle <= 240 && angle >= 120)
		return E_Directions::Backward;

	if (angle > 240 && angle < 345)
		return E_Directions::Right;

	if (angle > 15 && angle < 120)
		return E_Directions::Left;

	else return E_Directions::Backward;
}

void APlayerCharacterCombat::ListenToDodge(E_Directions moveDirection)
{
	if (moveDirection == E_Directions::None)
	{
		canMove = true;
	}
	else canMove = false;
}
#pragma endregion

//Logic specifically for heavy attacks
#pragma region HeavyAttack
void APlayerCharacterCombat::StartChargedAttack_Implementation()
{

}

void APlayerCharacterCombat::StopChargedAttack_Implementation()
{
	if (!IsValid(combatSystem) || !isCharging)
		return;

	combatSystem->StopCharge();
}

void APlayerCharacterCombat::ResetIsCharging()
{
	isCharging = false;
}

void APlayerCharacterCombat::ChargeAttack(E_ChargeSuccess success, int chargeCount)
{
	if (IsValid(weaponsSystem) && success == E_ChargeSuccess::Success)
		weaponsSystem->GetCurrentHeldWeapon()->StartSphereTrace(true, chargeCount);
	else
		ChargeAnimationDone();
}

void APlayerCharacterCombat::ChargeAnimationDone()
{
	canMove = true;
	isCharging = false;
}
#pragma endregion

#pragma region Damage Reactions
void APlayerCharacterCombat::ReactToDamage(E_DamageResponse damageResponse)
{
	switch (damageResponse)
	{
		default:
		case E_DamageResponse::None:
			canReadInput = true;
			break;
		case E_DamageResponse::Stagger:
			canReadInput = false;
			break;
	}
}
#pragma endregion

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
	if (gameState && lockOn)
	{
		lockedOnEnemy = gameState->GetClosestEnemy(this, enemyIndex);
	}
}

void APlayerCharacterCombat::FocusOnEnemy()
{
	if (lockedOnEnemy && enemyIndex > -1 && lockOn)
	{
		RotatePlayer();
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &APlayerCharacterCombat::FocusOnEnemy);
	}
}

void APlayerCharacterCombat::RotatePlayer()
{
	if (camera && IsValid(Controller) && lockOn)
	{
		//Get angle
		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(camera->GetComponentLocation(), lockedOnEnemy->GetActorLocation());

		FRotator newRotation = FMath::RInterpTo(GetActorRotation(), rotator, GetWorld()->DeltaTimeSeconds, rotateSpeed);

		Controller->SetControlRotation(FRotator(GetActorRotation().Pitch, newRotation.Yaw, GetActorRotation().Roll));
	}
}

void APlayerCharacterCombat::Look_Implementation(const FInputActionValue& value)
{
	FVector2D lookVector = value.Get<FVector2D>();

	if (camera->GetComponentRotation().Pitch > freeCameraRotationUpperLimit + lookVector.Y || camera->GetComponentRotation().Pitch < freeCameraRotationLowerLimit + lookVector.Y)
	{
		lookVector.Y = 0;
	}

	const FInputActionValue newValue(lookVector);

	if (!lockOn)
	{
		Super::Look_Implementation(newValue);

		FRotator cameraRotation = camera->GetComponentRotation();
		cameraRotation.Pitch = FMath::Clamp(cameraRotation.Pitch, freeCameraRotationLowerLimit, freeCameraRotationUpperLimit);

		camera->SetWorldRotation(cameraRotation);
	}
}
#pragma endregion

#pragma region Equipment
void APlayerCharacterCombat::GetEquipmentInfo()
{
	healthIncrease = 0;
	damageIncrease = 0;

	TArray<UPDA_Equipment*> equipment = inventory->GetEquippedEquipment();

	for (UPDA_Equipment* equip : equipment)
	{
		switch (equip->affectedStat)
		{
		case E_StatType::Health:
			healthIncrease += equip->affectedAmount;
			break;
		case E_StatType::Damage:
			damageIncrease += equip->affectedAmount;
			break;
		}
	}

	if (damageSystem)
	{
		damageSystem->IncreaseMaxHealth(healthIncrease);
	}

	if (weaponsSystem)
	{
		weaponsSystem->GetCurrentHeldWeapon()->SetDamageIncrease(damageIncrease);
	}
}
#pragma endregion

//Implementation of Attack interface
#pragma region AttacksInterface

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
	if (APlayerControllerBase* playerController = Cast<APlayerControllerBase>(GetController()))
	{
		canMove = false;

		playerController->PlayerDeath();
		GetCharacterMovement()->StopActiveMovement();
	}
	//GetWorld()->GetTimerManager().SetTimer(resetLevelTimer, this, &APlayerCharacterCombat::ResetCurrentLevel, 3, false);
}
#pragma endregion

#pragma region Endings
void APlayerCharacterCombat::ResetCurrentLevel()
{
	const FString CurrentMap = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentMap));
}

void APlayerCharacterCombat::CombatEnded(int exp, int gold)
{
	canMove = false;
	GetCharacterMovement()->StopActiveMovement();

	combatEnd = true;
	inventory->CombatEnd(exp, gold);

	if (IsValid(combatSystem))
		combatSystem->TearDown();

	if (IsValid(musicManager))
	{
		musicManager->OnBeat.RemoveDynamic(this, &APlayerCharacterCombat::CanPauseGame);
	}

	if (IsValid(dodgeComponent))
	{
		dodgeComponent->OnDodge.RemoveDynamic(this, &APlayerCharacterCombat::ListenToDodge);
	}
}
#pragma endregion

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
float APlayerCharacterCombat::LightAttack_Implementation(TArray<E_CombatActionType>& previousActions)
{
	return 0;
}

float APlayerCharacterCombat::HeavyAttack_Implementation(TArray<E_CombatActionType>& previousActions, bool charged)
{
	return 0;
}

float APlayerCharacterCombat::SpecialAttack_Implementation()
{
	return 0;
}
*/
#pragma endregion