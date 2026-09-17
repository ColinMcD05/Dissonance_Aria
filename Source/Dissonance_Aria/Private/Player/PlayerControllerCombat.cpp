// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerControllerCombat.h"
#include "EnhancedInputSubsystems.h"

void APlayerController::BeginPlay()
{

}

void APlayerControllerCombat::SetUpController()
{
	APlayerController* playerController = Cast<APlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
}