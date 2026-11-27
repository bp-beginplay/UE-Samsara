// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/BasePlayerController.h"

#include "Interface/TeamInterface.h"

#include "PvPPlayerController.generated.h"

class ABaseCharacter;
class UDA_PCInput;

/**
 * PvP 매치의 플레이어 컨트롤러.
 * 게임모드가 스폰한 아라한 목록을 들고, 숫자키나 UI 버튼으로 조종할 아라한을 바꾼다(빙의).
 */
UCLASS()
class SAMSARA_API APvPPlayerController : public ABasePlayerController, public ITeamInterface
{
	GENERATED_BODY()

	// 기본 함수
public:
	/** Arhats를 복제한다. */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	/** 숫자키 입력 매핑을 추가하고 아라한 선택 함수에 연결한다. */
	virtual void SetupInputComponent() override;

	// 커스텀 함수
public:
	/** 서버에서 ArhatIndex번 아라한에 빙의한다. 숫자키와 UI 버튼에서 호출. */
	UFUNCTION(Reliable, Server)
	void ServerPossessArhat(int32 ArhatIndex);
	/** 게임모드가 스폰한 아라한 목록을 넘겨받고 0번 아라한에 빙의한다. */
	UFUNCTION(Reliable, Server)
	void ServerSetArhats(const TArray<ABaseCharacter*>& InArhats);

private:
	// 입력 처리
	/** 1: 첫 번째 아라한에 빙의를 요청한다. */
	void OnSelectArhat1();
	/** 2: 두 번째 아라한에 빙의를 요청한다. */
	void OnSelectArhat2();

	/** HUD 메뉴의 아라한 버튼을 지금 아라한 목록으로 초기화한다. */
	void InitArhatsMenu();

	/** Arhats를 받으면(클라이언트) 모든 아라한이 도착했을 때 메뉴를 초기화한다. */
	UFUNCTION()
	void OnRep_Arhats();

	// 변수
private:
	/** 숫자키로 아라한을 고르는 입력 매핑과 입력 액션. */
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDA_PCInput* InputData;
	/** 이 플레이어가 조종할 수 있는 아라한들. */
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Arhats)
	TArray<ABaseCharacter*> Arhats;
	/** 소속 팀. 게임모드가 시작 위치를 고를 때 정한다. (서버에만 있음) */
	int32 TeamId = INDEX_NONE;

	// Get/Set 함수
public:
	// ITeamInterface
	virtual int32 GetTeamId() const override { return TeamId; }
	virtual void SetTeamId(int32 NewTeamId) override { TeamId = NewTeamId; }
};
