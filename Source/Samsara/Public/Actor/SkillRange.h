// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillRange.generated.h"

class UDecalComponent;

/** 소유 캐릭터의 이동 거리(MaxWalkSpeed)를 반지름으로 하는 원 데칼을 표시하는 액터. */
UCLASS()
class SAMSARA_API ASkillRange : public AActor
{
	GENERATED_BODY()

	// 기본 함수
public:
	ASkillRange();
protected:
	virtual void BeginPlay() override;

	// 변수
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDecalComponent> SkillRangeDecal;
};
