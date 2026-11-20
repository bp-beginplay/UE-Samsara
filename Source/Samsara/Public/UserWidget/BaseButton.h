// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseButton.generated.h"

class UButton;
class UTexture2D;

UCLASS()
class SAMSARA_API UBaseButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetButtonImage(UTexture2D* _Image);

public:
	UPROPERTY(meta = (BindWidget))
	UButton* Button;
};
