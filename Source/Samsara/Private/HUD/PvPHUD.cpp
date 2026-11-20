// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PvPHUD.h"

#include "UserWidget/PvP/PvPMainMenu.h"
#include "UserWidget/PvP/ArhatsBox.h"
#include "LogTest.h"

void APvPHUD::InitPvPMainMenu(TArray<ABaseCharacter*> _Arhats)
{
	PvPMainMenu = CreateWidget<UPvPMainMenu>(GetOwningPlayerController(), PvPMainMenuClass);
	PvPMainMenu->AddToViewport();
	PvPMainMenu->ArhatsBox->InitArhatButtons(_Arhats);
}