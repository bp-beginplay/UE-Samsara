// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Input/DA_ArhatInput.h"

#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"

void UDA_ArhatInput::AddMapping(const AController* Controller) const
{
	const APlayerController* PC = Cast<APlayerController>(Controller);
	if (!PC)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, MappingPriority);
	}
}

void UDA_ArhatInput::RemoveMapping(const AController* Controller) const
{
	const APlayerController* PC = Cast<APlayerController>(Controller);
	if (!PC)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(MappingContext);
	}
}
