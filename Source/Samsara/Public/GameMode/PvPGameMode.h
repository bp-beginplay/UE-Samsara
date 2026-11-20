// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PvPGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SAMSARA_API APvPGameMode : public AGameMode
{
	GENERATED_BODY()
	
protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual bool ReadyToStartMatch_Implementation();
	virtual void HandleMatchHasStarted();
};
