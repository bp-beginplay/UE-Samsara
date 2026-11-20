// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Input/DA_PCInput.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"

void UDA_PCInput::AddMappingPC(const ULocalPlayer* LocalPlayer) const
{
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
    {
        if (!Subsystem->HasMappingContext(IMC_PC))
        {
            Subsystem->AddMappingContext(IMC_PC, Pri_PC);
        }
    }
}

void UDA_PCInput::RemoveMappingContext(const ULocalPlayer* LocalPlayer) const
{
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
    {
        Subsystem->RemoveMappingContext(IMC_PC);
    }
}
