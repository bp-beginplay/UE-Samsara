// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PvPHUD.generated.h"

class UPvPMainMenu;
class ABaseCharacter;

/** PvP 매치의 HUD. 메인 메뉴 위젯(아라한 선택 버튼)을 만든다. */
UCLASS()
class SAMSARA_API APvPHUD : public AHUD
{
	GENERATED_BODY()

	// 커스텀 함수
public:
	/** 메인 메뉴를 만들고(처음 한 번) 아라한 버튼을 초기화한다. */
	void InitMainMenu(const TArray<ABaseCharacter*>& Arhats);

	// 변수
private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UPvPMainMenu> MainMenuClass;
	UPROPERTY()
	UPvPMainMenu* MainMenu;
};
