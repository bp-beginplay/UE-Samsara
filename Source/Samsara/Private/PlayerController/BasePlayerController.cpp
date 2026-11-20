// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/BasePlayerController.h"

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	//FInputModeGameAndUI InputModeData;
	//InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//InputModeData.SetHideCursorDuringCapture(false);
	//SetInputMode(InputModeData);
}

FHitResult ABasePlayerController::GetUnderCursorHit()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	return Hit;
}
