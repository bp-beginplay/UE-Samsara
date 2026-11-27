// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/PvP/ArhatsBox.h"

#include "Components/Button.h"

#include "Character/BaseCharacter.h"
#include "PlayerController/PvPPlayerController.h"
#include "UserWidget/PvP/ArhatButton.h"

void UArhatsBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ArhatButton1->GetButton()->OnClicked.AddDynamic(this, &UArhatsBox::ClickArhatButton1);
	ArhatButton2->GetButton()->OnClicked.AddDynamic(this, &UArhatsBox::ClickArhatButton2);
	ArhatButton3->GetButton()->OnClicked.AddDynamic(this, &UArhatsBox::ClickArhatButton3);
	ArhatButton4->GetButton()->OnClicked.AddDynamic(this, &UArhatsBox::ClickArhatButton4);
	ArhatButton5->GetButton()->OnClicked.AddDynamic(this, &UArhatsBox::ClickArhatButton5);
	ArhatButton6->GetButton()->OnClicked.AddDynamic(this, &UArhatsBox::ClickArhatButton6);
}

void UArhatsBox::InitArhatButtons(const TArray<ABaseCharacter*>& Arhats)
{
	const TArray<UArhatButton*> ArhatButtons = GetArhatButtons();

	// 버튼 수(6)보다 아라한이 많으면 넘치는 아라한은 표시하지 않는다.
	const int32 Count = FMath::Min(Arhats.Num(), ArhatButtons.Num());
	for (int32 i = 0; i < Count; ++i)
	{
		// 아직 복제되지 않은 아라한(null)은 건너뛴다.
		if (IsValid(Arhats[i]) && ArhatButtons[i])
		{
			ArhatButtons[i]->SetButtonImage(Arhats[i]->GetArhatIcons().ArhatIcon);
		}
	}
}

TArray<UArhatButton*> UArhatsBox::GetArhatButtons() const
{
	return { ArhatButton1, ArhatButton2, ArhatButton3, ArhatButton4, ArhatButton5, ArhatButton6 };
}

void UArhatsBox::RequestPossessArhat(int32 ArhatIndex)
{
	if (APvPPlayerController* PC = GetOwningPlayer<APvPPlayerController>())
	{
		PC->ServerPossessArhat(ArhatIndex);
	}
}

void UArhatsBox::ClickArhatButton1() { RequestPossessArhat(0); }
void UArhatsBox::ClickArhatButton2() { RequestPossessArhat(1); }
void UArhatsBox::ClickArhatButton3() { RequestPossessArhat(2); }
void UArhatsBox::ClickArhatButton4() { RequestPossessArhat(3); }
void UArhatsBox::ClickArhatButton5() { RequestPossessArhat(4); }
void UArhatsBox::ClickArhatButton6() { RequestPossessArhat(5); }
