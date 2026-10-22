


#include "CombatSystem/DodgeComponent.h"
#include "Player/PlayerCharacterCombat.h"
#include "KismetAnimationLibrary.h"
#include "Music/MusicManagerCombat.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UDodgeComponent::UDodgeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDodgeComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<APlayerCharacterCombat>(GetOwner());

	musicManager = Cast<AMusicManagerCombat>(UGameplayStatics::GetActorOfClass(GetWorld(), AMusicManagerCombat::StaticClass()));
}

E_Directions UDodgeComponent::CalculateDodgeDirection()
{
	if (!IsValid(player))
		return E_Directions::Forward;

	float direction = UKismetAnimationLibrary::CalculateDirection(player->GetVelocity(), player->GetActorRotation());

	if (direction == 0 || (direction <= -110 && direction <= 60))
		return E_Directions::Backward;

	if (direction > -110 && direction < 0)
		return E_Directions::Left;

	if (direction < 180 && direction > 60)
		return E_Directions::Right;

	else return E_Directions::Backward;
}

void UDodgeComponent::StartDodge(E_Directions direction)
{
	if (dodging || !IsValid(musicManager))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString::Printf(TEXT("dodging: %d, valid: %d"), dodging, IsValid(musicManager)));
		return;
	}

	if (direction == E_Directions::None)
		direction = CalculateDodgeDirection();

	dodging = true;

	if (musicManager->GetBPM() <= 0.)
	{
		EndDodge();
		return;
	}

	timeToDodge = (60 / musicManager->GetBPM()) * 0.5;

	OnDodge.Broadcast(direction);

	float timing = FMath::Abs(0.5 - musicManager->GetHowCloseToBeat()) + 0.5f;
	float distance = baseDistance;

	switch (direction)
	{
		case E_Directions::Left:
			endPoint = player->GetActorRightVector() * -1;
			break;
		case E_Directions::Backward:
			endPoint = player->GetActorForwardVector() * -1;
			break;
		case E_Directions::Right:
			endPoint = player->GetActorRightVector();
			break;
		default:
		case E_Directions::None:
			dodging = false;
			break;
	}

	if (timing < 0.5 + perfectDodgeRange)
	{
		distance = baseDistance * perfectDodgeMult;
	}
	else
	{
		distance = baseDistance * (timing + 0.2);
	}

	endPoint = player->GetActorLocation() + distance * endPoint;

	speed = distance/timeToDodge;

	Dodge();
}

void UDodgeComponent::Dodge()
{
	if (!IsValid(player))
	{
		EndDodge();
		return;
	}

	if (player->GetActorLocation().Equals(endPoint, 0.1f))
	{
		player->SetActorLocation(endPoint);
		EndDodge();
	}
	else 
	{
		FVector newPosition = FMath::VInterpConstantTo(player->GetActorLocation(), endPoint, GetWorld()->DeltaTimeSeconds, speed);
		
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString::SanitizeFloat(newPosition.X).Append(FString::SanitizeFloat(newPosition.Y)).Append(FString::SanitizeFloat(newPosition.Z)));

		player->SetActorLocation(newPosition);
		
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UDodgeComponent::Dodge);
	}
}

void UDodgeComponent::EndDodge()
{
	OnDodge.Broadcast(E_Directions::None);
	dodging = false;
}

void UDodgeComponent::ResetDodge()
{
	OnDodge.Broadcast(E_Directions::None);
	dodging = false;
}