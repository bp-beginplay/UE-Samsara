// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ArhatAnimInstance.generated.h"

class ABaseCharacter;
class UCharacterMovementComponent;

/** 아라한 애니메이션 블루프린트의 부모 클래스. 이동 애니메이션(Idle/Jog)에 쓸 값을 매 프레임 계산한다. */
UCLASS()
class SAMSARA_API UArhatAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	// 기본 함수
public:
	/** 소유 캐릭터와 무브먼트 컴포넌트를 찾아 둔다. */
	virtual void NativeInitializeAnimation() override;
	/** 매 프레임 GroundSpeed, bHasAcceleration을 갱신한다. (워커 스레드에서 실행) */
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	// 변수
protected:
	/** 이 애니메이션을 쓰는 캐릭터. */
	UPROPERTY()
	ABaseCharacter* OwningCharacter;
	/** 소유 캐릭터의 무브먼트 컴포넌트. 가속 여부를 읽는다. */
	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;

	/** 평면 이동 속도. 블렌드 스페이스(Idle ↔ Jog)의 입력. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;
	/** 이동 입력(가속)이 있는지. 출발/정지 애니메이션 전환에 쓴다. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;
};
