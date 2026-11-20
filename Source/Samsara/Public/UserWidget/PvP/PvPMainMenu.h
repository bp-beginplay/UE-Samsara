// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PvPMainMenu.generated.h"

class UArhatsBox;

UCLASS()
class SAMSARA_API UPvPMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UArhatsBox* ArhatsBox;
};
