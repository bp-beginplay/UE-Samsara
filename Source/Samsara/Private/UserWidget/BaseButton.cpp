// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/BaseButton.h"

#include "Components/Button.h"

void UBaseButton::SetButtonImage(UTexture2D* InImage)
{
	if (!Button)
	{
		return;
	}

	FButtonStyle Style = Button->GetStyle();
	Style.Normal.SetResourceObject(InImage);
	Style.Hovered.SetResourceObject(InImage);
	Style.Pressed.SetResourceObject(InImage);
	Button->SetStyle(Style);
}
