// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

/** 모든 플레이어 컨트롤러의 공통 클래스. 마우스 커서를 보이게 하고 커서 아래 판정을 제공한다. */
UCLASS()
class SAMSARA_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

	// 기본 함수
protected:
	virtual void BeginPlay() override;

	// 커스텀 함수
public:
	/** 마우스 커서 아래를 Visibility 채널로 판정한 결과. */
	FHitResult GetUnderCursorHit() const;
};
