// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "RangeComponent.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class ITeamInterface;

/**
 * 사거리 컴포넌트.
 *
 * 스피어 반지름으로 사거리를 나타내고, 사거리 안에 들어온 적과 이 컴포넌트를 가진 액터를
 * 연결 이펙트(나이아가라 빔)로 잇는다.
 * 적·아군 판정은 소유 액터의 Instigator(없으면 소유 액터)의 팀으로 한다.
 *  - 캐릭터에 붙인 경우: 폰은 자기 자신이 Instigator이므로 캐릭터의 팀
 *  - 고스트에 붙인 경우: 고스트를 스폰한 캐릭터(Instigator)의 팀
 * 표시 전용이므로 조종하는 플레이어의 머신에서만 Active를 호출한다.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SAMSARA_API URangeComponent : public USphereComponent
{
	GENERATED_BODY()

	// 기본 함수
public:
	URangeComponent();
protected:
	virtual void BeginPlay() override;
	/** 소유 액터가 파괴될 때 월드에 남는 연결 이펙트를 정리한다. */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// 커스텀 함수
public:
	/** 사거리를 RangeRadius로 설정하고 오버랩 감지를 켠다. */
	void Active();
	/** 오버랩 감지를 끄고 연결 이펙트를 모두 지운다. */
	void DeActive();
private:
	UFUNCTION()
	void BeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	/** 적·아군 판정에 쓸 팀. 소유 액터의 Instigator, 없으면 소유 액터. */
	const ITeamInterface* GetTeamSource() const;

	/** 소유 액터에서 Target까지 이어지는 연결 이펙트를 만든다. */
	void SpawnTargetLinkEffect(const AActor* Target);
	/** 만들어 둔 연결 이펙트를 모두 지운다. */
	void ClearTargetLinkEffects();

	// 변수
private:
	/** Active 시 설정하는 사거리(스피어 반지름). */
	UPROPERTY(EditDefaultsOnly, Category = "Range", meta = (ClampMin = "0.0"))
	float RangeRadius = 400.f;

	/**
	 * 사거리 안의 적과 소유 캐릭터를 잇는 빔 나이아가라 시스템.
	 * 시스템에 Vector 타입 User 파라미터 BeamStart, BeamEnd가 있어야 한다.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem> TargetLinkEffect;
	/** 지금 화면에 떠 있는 연결 이펙트들. */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UNiagaraComponent>> TargetLinkEffects;

	// Get/Set 함수
public:
	void SetRangeRadius(float NewRadius) { RangeRadius = NewRadius; }
	float GetRangeRadius() const { return RangeRadius; }
};
