// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TeamInterface.generated.h"

UINTERFACE(MinimalAPI)
class UTeamInterface : public UInterface
{
	GENERATED_BODY()
};

/** 팀을 가진 대상(캐릭터, 컨트롤러, 시작 위치)이 구현한다. TeamId가 INDEX_NONE이면 팀 없음. */
class SAMSARA_API ITeamInterface
{
	GENERATED_BODY()

public:
	virtual int32 GetTeamId() const = 0;
	virtual void SetTeamId(int32 NewTeamId) = 0;

	/** 같은 팀인지. 둘 중 하나라도 팀이 없으면 false. */
	bool IsAlly(const AActor* Other) const;
	/** 다른 팀인지. 둘 중 하나라도 팀이 없으면 false. */
	bool IsEnemy(const AActor* Other) const;
};
