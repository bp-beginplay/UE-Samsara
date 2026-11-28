// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "RangeComponent.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class ABaseCharacter;

/**
 * 공격·스킬 사거리 스피어.
 * 감지가 켜져 있는 동안 범위 안에 들어온 캐릭터를 소유 액터와 빔 이펙트로 잇는다.
 * 콜리전 프리셋은 "Range" (Pawn만 Overlap).
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SAMSARA_API URangeComponent : public USphereComponent
{
	GENERATED_BODY()

	// 기본 함수
public:
	/** "Range" 콜리전 프리셋을 쓰고, 게임 중에도 원이 보이게 한다. */
	URangeComponent();
	/** 이펙트 위치를 캐릭터 위치에 맞춘다. */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	/** 오버랩 이벤트를 연결한다. */
	virtual void BeginPlay() override;
	/** 소유 액터가 파괴될 때 월드에 남는 이펙트를 정리한다. */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// 커스텀 함수
public:
	/** 범위 감지를 켠다. (기본은 NoCollision이라 감지하지 않음) */
	void EnableDetection();
	/** 사거리(스피어 반지름)를 바꾼다. */
	void SetRangeRadius(float InRadius);

private:
	/** 캐릭터가 사거리 안에 들어오면 대상에 추가한다. */
	UFUNCTION()
	void OnRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/** 캐릭터가 사거리 밖으로 나가면 대상에서 뺀다. */
	UFUNCTION()
	void OnRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	/** 범위 안에 들어온 캐릭터를 추가하고 그 캐릭터로 이어지는 이펙트를 만든다. */
	void AddTarget(ABaseCharacter* Target);
	/** 범위에서 나간 캐릭터와 그 이펙트를 지운다. */
	void RemoveTarget(ABaseCharacter* Target);
	/** 이펙트 시작점과 끝점을 소유 액터와 각 캐릭터 위치로 맞춘다. */
	void UpdateTargetLinkEffects();
	/** 이펙트를 모두 지우고 대상 목록을 비운다. */
	void ClearTargetLinkEffects();

	// 변수
private:
	/** 범위 안 캐릭터와 잇는 빔 나이아가라 시스템. Vector 타입 User 파라미터 BeamStart, BeamEnd가 있어야 한다. */
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem> TargetLinkEffectSystem;

	/** 범위 안 캐릭터. TargetLinkEffectComponents와 같은 인덱스끼리 짝이다. */
	UPROPERTY(Transient)
	TArray<TObjectPtr<ABaseCharacter>> TargetsInRange;
	/** 범위 안 캐릭터마다 하나씩 만든 빔 이펙트. */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UNiagaraComponent>> TargetLinkEffectComponents;

	// Get/Set 함수
public:
	/** 사거리 안에 있는 아군이 아닌 캐릭터들. */
	const TArray<TObjectPtr<ABaseCharacter>>& GetTargetsInRange() const { return TargetsInRange; }
};
