// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PvPHUD.generated.h"

class UPvPMainMenu;
class ABaseCharacter;

UCLASS()
class SAMSARA_API APvPHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void InitPvPMainMenu(TArray<ABaseCharacter*> _Arhats);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UPvPMainMenu> PvPMainMenuClass;
	UPROPERTY()
	UPvPMainMenu* PvPMainMenu;
};
