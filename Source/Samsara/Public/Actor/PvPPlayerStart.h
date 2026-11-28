// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"

#include "Interface/TeamInterface.h"

#include "PvPPlayerStart.generated.h"

/** 팀이 정해진 PvP 시작 위치. 레벨에 배치하고 Team을 지정한다. */
UCLASS()
class SAMSARA_API APvPPlayerStart : public APlayerStart, public ITeamInterface
{
	GENERATED_BODY()

	// 변수
private:
	/** 이 시작 위치를 쓰는 팀. 레벨에서 지정한다. */
	UPROPERTY(EditAnywhere, Category = "Team")
	ETeam Team = ETeam::None;
	/** 이미 한 플레이어가 이 시작 위치를 쓰고 있는지. */
	bool bSpawned = false;

	// Get/Set 함수
public:
	/** 이미 쓰인 시작 위치인지. */
	bool IsSpawned() const { return bSpawned; }
	/** 시작 위치를 쓰였다고 표시한다. */
	void SetSpawned(bool bNewSpawned) { bSpawned = bNewSpawned; }

	// ITeamInterface
	virtual ETeam GetTeam() const override { return Team; }
	virtual void SetTeam(ETeam NewTeam) override { Team = NewTeam; }
};
