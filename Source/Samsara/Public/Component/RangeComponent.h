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
	URangeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;
	/** 소유 액터가 파괴될 때 월드에 남는 이펙트를 정리한다. */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// 커스텀 함수
public:
	/** 범위 감지를 켠다. */
	void EnableDetection();
	/** 범위 감지를 끄고 이펙트를 모두 지운다. */
	void DisableDetection();
	/** 사거리(스피어 반지름)를 바꾼다. */
	void SetRangeRadius(float InRadius);

private:
	UFUNCTION()
	void OnRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	/** 범위 안에 들어온 캐릭터를 추가하고 그 캐릭터로 이어지는 이펙트를 만든다. */
	void AddTarget(ABaseCharacter* Target);
	/** 범위에서 나간 캐릭터와 그 이펙트를 지운다. */
	void RemoveTarget(ABaseCharacter* Target);
	/** 이펙트 시작점과 끝점을 소유 액터와 각 캐릭터 위치로 맞춘다. */
	void UpdateTargetLinkEffects();
	void ClearTargetLinkEffects();

	// 변수
private:
	/** 범위 안 캐릭터와 잇는 빔 나이아가라 시스템. Vector 타입 User 파라미터 BeamStart, BeamEnd가 있어야 한다. */
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem> TargetLinkEffectSystem;

	/** 범위 안 캐릭터와, 그 캐릭터로 이어지는 이펙트. 같은 인덱스끼리 짝이다. */
	UPROPERTY(Transient)
	TArray<TObjectPtr<ABaseCharacter>> TargetsInRange;
	UPROPERTY(Transient)
	TArray<TObjectPtr<UNiagaraComponent>> TargetLinkEffectComponents;

	// Get/Set 함수
public:
	const TArray<TObjectPtr<ABaseCharacter>>& GetTargetsInRange() const { return TargetsInRange; }
};
