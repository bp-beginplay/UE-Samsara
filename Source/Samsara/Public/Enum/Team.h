// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Team.generated.h"

/** 소속 팀. 캐릭터, 플레이어 컨트롤러, 시작 위치가 가진다. */
UENUM(BlueprintType)
enum class ETeam : uint8
{
	/** 팀 없음. 어느 팀과도 같은 편이 아니다. */
	None	UMETA(DisplayName = "None"),
	/** 1팀 */
	Team1	UMETA(DisplayName = "Team 1"),
	/** 2팀 */
	Team2	UMETA(DisplayName = "Team 2"),
};
