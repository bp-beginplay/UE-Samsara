// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/BasePlayerController.h"

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

FHitResult ABasePlayerController::GetUnderCursorHit() const
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	return Hit;
}
