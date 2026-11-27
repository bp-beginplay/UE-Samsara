// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PvPMainMenu.generated.h"

class UArhatsBox;

/** PvP 매치의 메인 메뉴 위젯. */
UCLASS()
class SAMSARA_API UPvPMainMenu : public UUserWidget
{
	GENERATED_BODY()

	// 변수
private:
	UPROPERTY(meta = (BindWidget))
	UArhatsBox* ArhatsBox;

	// Get/Set 함수
public:
	UArhatsBox* GetArhatsBox() const { return ArhatsBox; }
};
