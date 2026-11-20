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

class SAMSARA_API ITeamInterface
{
	GENERATED_BODY()

public:
    virtual int32 GetTeamId() const = 0;
    virtual void SetTeamId(int32 NewTeamId) = 0;

    bool IsAlly(const AActor* Other) const;
    bool IsEnemy(const AActor* Other) const;
};
