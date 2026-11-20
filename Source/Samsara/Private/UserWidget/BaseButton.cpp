// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/BaseButton.h"

#include "Components/Button.h"

void UBaseButton::SetButtonImage(UTexture2D* _Image)
{
	FButtonStyle Style = Button->GetStyle();
	Style.Normal.SetResourceObject(_Image);
	Style.Hovered.SetResourceObject(_Image);
	Style.Pressed.SetResourceObject(_Image);
	Button->SetStyle(Style);
}