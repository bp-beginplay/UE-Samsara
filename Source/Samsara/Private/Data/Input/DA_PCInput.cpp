// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Input/DA_PCInput.h"

#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"

void UDA_PCInput::AddMapping(const ULocalPlayer* LocalPlayer) const
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		if (!Subsystem->HasMappingContext(MappingContext))
		{
			Subsystem->AddMappingContext(MappingContext, MappingPriority);
		}
	}
}

void UDA_PCInput::RemoveMapping(const ULocalPlayer* LocalPlayer) const
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		Subsystem->RemoveMappingContext(MappingContext);
	}
}
