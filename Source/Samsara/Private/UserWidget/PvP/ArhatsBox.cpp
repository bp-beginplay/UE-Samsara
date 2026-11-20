// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/PvP/ArhatsBox.h"

#include "Components/Button.h"

#include "Character/BaseCharacter.h"
#include "UserWidget/PvP/ArhatButton.h"
#include "PlayerController/PvPPlayerController.h"

void UArhatsBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ArhatButton1->Button->OnClicked.AddDynamic(this, &UArhatsBox::ClickArhatButton1);
	ArhatButton2->Button->OnClicked.AddDynamic(this, &UArhatsBox::ClickArhatButton2);
	ArhatButton3->Button->OnClicked.AddDynamic(this, &UArhatsBox::ClickArhatButton3);
	ArhatButton4->Button->OnClicked.AddDynamic(this, &UArhatsBox::ClickArhatButton4);
	ArhatButton5->Button->OnClicked.AddDynamic(this, &UArhatsBox::ClickArhatButton5);
	ArhatButton6->Button->OnClicked.AddDynamic(this, &UArhatsBox::ClickArhatButton6);
}

void UArhatsBox::InitArhatButtons(TArray<ABaseCharacter*> _Arhats)
{
	TArray<UArhatButton*> ArhatButtons = { ArhatButton1, ArhatButton2, ArhatButton3, ArhatButton4, ArhatButton5, ArhatButton6 };
	int Index = 0;
	for (auto& iter : _Arhats)
	{
		ArhatButtons[Index]->SetButtonImage(iter->GetArhatIcons().ArhatIcon);
		Index++;
	}
}

void UArhatsBox::ClickArhatButton1()
{
	ClickArhat(0);
}

void UArhatsBox::ClickArhatButton2()
{
	ClickArhat(1);
}

void UArhatsBox::ClickArhatButton3()
{
	ClickArhat(2);
}

void UArhatsBox::ClickArhatButton4()
{
	ClickArhat(3);
}

void UArhatsBox::ClickArhatButton5()
{
	ClickArhat(4);
}

void UArhatsBox::ClickArhatButton6()
{
	ClickArhat(6);
}

void UArhatsBox::ClickArhat(int _Index)
{
	if (APvPPlayerController* PC = GetOwningPlayer<APvPPlayerController>())
	{
		PC->ServerPossess(_Index);
	}
}