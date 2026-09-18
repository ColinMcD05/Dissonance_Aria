// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerControllerCombat.h"
#include "EnhancedInputSubsystems.h"

void APlayerControllerCombat::BeginPlay()
{
	Super::BeginPlay();
	SetUpController();

}

void APlayerControllerCombat::SetUpController()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer());
}