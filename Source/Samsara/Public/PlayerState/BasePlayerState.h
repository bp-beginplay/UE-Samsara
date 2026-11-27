// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "GameplayTagContainer.h"

#include "BasePlayerState.generated.h"

/** 모든 플레이어 스테이트의 공통 클래스. 플레이어가 고른 아라한 목록을 가진다. */
UCLASS()
class SAMSARA_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()

	// 변수
private:
	/** 플레이어가 고른 아라한 (DA_ArhatClass에서 클래스를 찾는 태그). */
	UPROPERTY(EditAnywhere)
	TArray<FGameplayTag> ArhatTags;

	// Get/Set 함수
public:
	const TArray<FGameplayTag>& GetArhatTags() const { return ArhatTags; }
};
