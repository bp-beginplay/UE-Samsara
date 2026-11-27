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
	/** 아라한 선택 버튼 묶음. (BindWidget) */
	UPROPERTY(meta = (BindWidget))
	UArhatsBox* ArhatsBox;

	// Get/Set 함수
public:
	/** 아라한 선택 버튼 묶음. HUD가 버튼을 초기화할 때 쓴다. */
	UArhatsBox* GetArhatsBox() const { return ArhatsBox; }
};
