// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"

#include "Interface/TeamInterface.h"

#include "PvPPlayerStart.generated.h"

/** 팀이 정해진 PvP 시작 위치. 레벨에 배치하고 TeamId를 지정한다. */
UCLASS()
class SAMSARA_API APvPPlayerStart : public APlayerStart, public ITeamInterface
{
	GENERATED_BODY()

	// 변수
private:
	UPROPERTY(EditAnywhere)
	int32 TeamId = INDEX_NONE;
	/** 이미 한 플레이어가 이 시작 위치를 쓰고 있는지. */
	bool bSpawned = false;

	// Get/Set 함수
public:
	bool IsSpawned() const { return bSpawned; }
	void SetSpawned(bool bNewSpawned) { bSpawned = bNewSpawned; }

	// ITeamInterface
	virtual int32 GetTeamId() const override { return TeamId; }
	virtual void SetTeamId(int32 NewTeamId) override { TeamId = NewTeamId; }
};
