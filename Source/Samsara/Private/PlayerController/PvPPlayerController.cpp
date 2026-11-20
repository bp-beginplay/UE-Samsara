// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/PvPPlayerController.h"

#include "Net/UnrealNetwork.h"

#include "EnhancedInputSubsystems.h"
#include "PlayerState/PvPPlayerState.h"
#include "GameInstance/SamsaraGameInstance.h"
#include "Character/BaseCharacter.h"
#include "Data/Input/DA_PCInput.h"
#include "LogTest.h"
#include "HUD/PvPHUD.h"

void APvPPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APvPPlayerController, Arhats);
}

void APvPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	DA_PCInput->AddMappingPC(GetLocalPlayer());
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EBindInput::Started(Input, DA_PCInput->IA_PC1, this, &APvPPlayerController::Possess1);
		EBindInput::Started(Input, DA_PCInput->IA_PC2, this, &APvPPlayerController::Possess2);
	}
}

void APvPPlayerController::ServerPossess_Implementation(int _Index)
{
	Possess(Arhats[_Index]);
}

void APvPPlayerController::Possess1()
{
	ServerPossess(0);
}

void APvPPlayerController::Possess2()
{
	ServerPossess(1);
}

void APvPPlayerController::ServerSetArhats_Implementation(const TArray<ABaseCharacter*>& _Arhats)
{
	Arhats = _Arhats;
	ServerPossess(0);
	if (APvPHUD* HUD = GetHUD<APvPHUD>())
	{
		HUD->InitPvPMainMenu(Arhats);
	}
}

void APvPPlayerController::OnRep_Arhats()
{
	if (APvPHUD* HUD = GetHUD<APvPHUD>())
	{
		HUD->InitPvPMainMenu(Arhats);
	}
}
