// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Enum/Team.h"

#include "TeamInterface.generated.h"

/** 리플렉션용 인터페이스 클래스. 실제 함수는 ITeamInterface에 있다. */
UINTERFACE(MinimalAPI)
class UTeamInterface : public UInterface
{
	GENERATED_BODY()
};

/** 팀을 가진 대상(캐릭터, 컨트롤러, 시작 위치)이 구현한다. ETeam::None이면 팀 없음. */
class SAMSARA_API ITeamInterface
{
	GENERATED_BODY()

public:
	/** 소속 팀. ETeam::None이면 팀 없음. */
	virtual ETeam GetTeam() const = 0;
	/** 소속 팀을 정한다. */
	virtual void SetTeam(ETeam NewTeam) = 0;

	/** 같은 팀인지. 둘 중 하나라도 팀이 없으면 false. */
	bool IsAlly(const AActor* Other) const;
};
