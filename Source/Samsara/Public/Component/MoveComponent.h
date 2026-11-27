// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class AGhostActor;

DECLARE_DELEGATE(FMoveTickDelegate);

/**
 * 턴제 이동 컴포넌트.
 *
 * 1) BeginSelectDestination : 목적지 고르기 시작. 매 틱 커서를 따라 경로 미리보기(경로 이펙트 + 고스트)를 갱신한다.
 * 2) ConfirmDestination     : 지금 미리보기 경로를 목적지로 확정한다.
 * 3) StartMove              : 확정한 경로(PathPoints)를 따라 이동을 시작한다.
 *
 * 틱에서는 TickFunction에 바인딩된 함수(미리보기 또는 이동) 하나만 실행한다.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SAMSARA_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

	// 기본 함수
public:
	UMoveComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 커스텀 함수
public:
	/** 목적지 고르기를 시작한다. */
	void BeginSelectDestination();
	/** 지금 미리보기 경로를 목적지로 확정한다. 고스트가 없으면(경로가 없으면) false. */
	bool ConfirmDestination();
	/** 확정한 목적지로 이동을 시작한다. 확정한 목적지가 없으면 false. */
	bool StartMove();

private:
	// 목적지 고르기 (미리보기)
	void UpdateDestinationPreview();
	/**
	 * 커서 위치로 가는 미리보기 경로를 PathPoints에 만든다.
	 * 커서가 장애물 위, 다른 캐릭터 안처럼 도착할 수 없는 곳을 가리켜도 그 근처의 갈 수 있는 곳까지 경로를 만든다.
	 *  1) 커서 위치로 바로 길찾기 (도착 불가면 엔진이 갈 수 있는 가장 가까운 곳까지의 부분 경로를 돌려줌)
	 *  2) 실패하면 커서 위치를 넓은 범위에서 가장 가까운 내비메시 지점으로 옮겨 다시 길찾기
	 *  3) 그래도 실패하면 캐릭터에서 커서 방향으로 내비메시를 따라가다 막히는 지점까지
	 * 모두 실패하면 false (PathPoints는 바꾸지 않음).
	 */
	bool BuildPreviewPath(AActor* Agent, const FVector& CursorLocation);
	/** PathPoints가 MaxLength보다 길면 경로를 따라 정확히 MaxLength 지점에서 자른다. */
	void TrimPathToLength(float MaxLength);
	/** PathPoints의 이웃한 두 지점마다 빔 이펙트를 하나씩 배치한다. */
	void UpdatePathEffects();
	void SpawnGhostActor();
	void DestroyGhostActor();
	void DestroyPathEffects();

	// 이동
	/** 매 틱 다음 경로 지점을 향해 이동 입력을 넣는다. */
	void UpdateMove();
	void FinishMove();

	/** 소유 캐릭터의 캡슐을 내비게이션 장애물로 쓸지 정한다. 목적지를 고르고 이동하는 동안에는 자기 장애물을 끈다. */
	void SetOwnerNavObstacle(bool bEnable);

	// 변수
private:
	// 설정
	/** 경로 구간마다 그리는 빔 나이아가라 시스템. Vector 타입 User 파라미터 BeamStart, BeamEnd가 있어야 한다. */
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem> PathEffectSystem;
	/** 바닥에 가려지지 않도록 경로 이펙트를 띄우는 높이. */
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float PathEffectHeightOffset = 10.f;
	UPROPERTY(EditDefaultsOnly, Category = "Ghost")
	TSubclassOf<AGhostActor> GhostActorClass;
	/** 이 거리(평면) 안에 들어오면 그 경로 지점에 도착한 것으로 본다. */
	UPROPERTY(EditDefaultsOnly, Category = "Move", meta = (ClampMin = "1.0"))
	float AcceptanceRadius = 30.f;
	/** 커서가 도착할 수 없는 곳을 가리킬 때, 가장 가까운 내비메시 지점을 찾는 범위 (가로, 세로, 높이 절반). */
	UPROPERTY(EditDefaultsOnly, Category = "Move")
	FVector UnreachableProjectExtent = FVector(300.f, 300.f, 500.f);

	// 상태
	/** 미리보기 또는 이동 경로 지점 (월드 좌표). */
	TArray<FVector> PathPoints;
	/** 경로 구간마다 하나씩 만든 빔 이펙트. 구간 i = PathPoints[i] → PathPoints[i + 1]. */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UNiagaraComponent>> PathEffectComponents;
	/** 목적지에 놓는 고스트. */
	UPROPERTY(Transient)
	TObjectPtr<AGhostActor> GhostActor;
	/** 매 틱 실행할 함수 (미리보기 또는 이동). */
	FMoveTickDelegate TickFunction;
	/** 목적지가 확정되어 이동할 수 있는 상태인지. */
	bool bHasDestination = false;
	/** 이동 중 지금 향하고 있는 PathPoints의 인덱스. */
	int32 TargetPointIndex = 0;

	// Get/Set 함수
public:
	const TArray<FVector>& GetPathPoints() const { return PathPoints; }
	AGhostActor* GetGhostActor() const { return GhostActor; }
};
