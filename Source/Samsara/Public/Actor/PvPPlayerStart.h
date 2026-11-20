// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"

#include "Interface/TeamInterface.h"

#include "PvPPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class SAMSARA_API APvPPlayerStart : public APlayerStart, public ITeamInterface
{
	GENERATED_BODY()
	
public:
	bool IsSpawn = false;

private:
	UPROPERTY(EditAnywhere)
	int32 TeamId = INDEX_NONE;
public:
	virtual int32 GetTeamId() const override { return TeamId; }
	virtual void SetTeamId(int32 NewTeamId) override { TeamId = NewTeamId; }
};
