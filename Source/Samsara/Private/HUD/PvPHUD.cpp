// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PvPHUD.h"

#include "UserWidget/PvP/ArhatsBox.h"
#include "UserWidget/PvP/PvPMainMenu.h"

void APvPHUD::InitMainMenu(const TArray<ABaseCharacter*>& Arhats)
{
	// 위젯은 처음 한 번만 만든다. 다시 불리면 버튼만 갱신한다.
	if (!MainMenu)
	{
		if (!MainMenuClass)
		{
			return;
		}
		MainMenu = CreateWidget<UPvPMainMenu>(GetOwningPlayerController(), MainMenuClass);
		if (!MainMenu)
		{
			return;
		}
		MainMenu->AddToViewport();
	}

	if (UArhatsBox* ArhatsBox = MainMenu->GetArhatsBox())
	{
		ArhatsBox->InitArhatButtons(Arhats);
	}
}
