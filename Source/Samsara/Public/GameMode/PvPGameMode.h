// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PvPGameMode.generated.h"

class ABaseCharacter;
class APvPPlayerController;
class APvPPlayerStart;

/**
 * 1대1 PvP 매치 게임모드.
 * 플레이어 2명이 모두 아라한을 고르면 매치를 시작하고, 각 플레이어의 아라한을 팀 시작 위치에 스폰한다.
 */
UCLASS()
class SAMSARA_API APvPGameMode : public AGameMode
{
	GENERATED_BODY()

	// 기본 함수
public:
	/** 아직 쓰지 않은 PvPPlayerStart를 골라 주고, 그 PlayerStart의 팀을 플레이어 팀으로 정한다. */
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	/** 플레이어 2명이 모두 접속해 아라한을 골랐으면 시작한다. */
	virtual bool ReadyToStartMatch_Implementation() override;
protected:
	/** 매치가 시작되면 플레이어마다 아라한을 스폰해 컨트롤러에 넘긴다. */
	virtual void HandleMatchHasStarted() override;

	// 커스텀 함수
private:
	/** 플레이어 컨트롤러와 같은 팀의 PlayerStart를 찾는다. 없으면 nullptr. */
	APvPPlayerStart* FindTeamPlayerStart(const APvPPlayerController* PC) const;
	/** 플레이어가 고른 아라한들을 팀 시작 위치에 스폰하고 팀을 정해 준다. */
	TArray<ABaseCharacter*> SpawnArhats(APvPPlayerController* PC, const APvPPlayerStart* PlayerStart) const;
};
