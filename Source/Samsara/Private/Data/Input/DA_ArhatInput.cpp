// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Input/DA_ArhatInput.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"

void UDA_ArhatInput::AddMappingArhat(const AController* Controller) const
{
    const APlayerController* PC = Cast<APlayerController>(Controller);

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(IMC_Arhat, Pri_Arhat);
    }
}

void UDA_ArhatInput::RemoveMappingArhat(const AController* Controller) const
{
    const APlayerController* PC = Cast<APlayerController>(Controller);
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
    {
        Subsystem->RemoveMappingContext(IMC_Arhat);
    }
}
