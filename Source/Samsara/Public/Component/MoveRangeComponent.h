// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DecalComponent.h"
#include "MoveRangeComponent.generated.h"

class UTexture2D;
class UMaterialInstanceDynamic;

/**
 * 캐릭터가 이번 턴에 실제로 이동할 수 있는 범위를 바닥에 표시하는 데칼.
 *
 * 이동 규칙(MoveComponent)과 같은 기준으로 계산한다.
 *  - 캐릭터 위치에서 각 지점까지 내비게이션 경로를 찾고
 *  - PathPoints 구간 길이의 합이 최대 이동 속도(GetMaxSpeed) 이내면 이동 가능
 *
 * 1) 주변을 GridResolution x GridResolution 칸으로 나눠 칸마다 위 검사를 한다.
 *    턴제라 목적지를 고르는 동안 주변이 변하지 않으므로, Show 후 StartDelay 뒤에 한 번에 전부 계산한다.
 * 2) 칸 결과를 TextureResolution 크기 텍스처로 옮길 때
 *    - 칸 사이를 부드럽게 보간하고 경계를 다시 또렷하게 만들어 계단 모양을 없애고
 *    - 바깥 원(반지름 = 이동 거리)은 칸과 상관없이 픽셀 단위로 정확하게 잘라 둥글게 만든다.
 * 3) 결과(이동 가능 = 흰색, 불가 = 검은색)를 데칼 머티리얼의 텍스처 파라미터(MaskParameterName)에 넘긴다.
 *
 * 데칼 머티리얼은 블루프린트의 Decal Material에 지정하고,
 * 그 머티리얼에 이름이 MaskParameterName인 Texture Parameter가 있어야 한다.
 * 표시 전용이므로 조종하는 플레이어의 화면에서만 Show를 호출한다.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SAMSARA_API UMoveRangeComponent : public UDecalComponent
{
	GENERATED_BODY()

	// 기본 함수
public:
	/** 데칼을 아래로 향하게 돌리고 숨긴 상태로 시작한다. */
	UMoveRangeComponent();

	// 커스텀 함수
public:
	/** 현재 위치를 중심으로 이동 가능 범위를 계산해 표시한다. 계산은 StartDelay 뒤에 한 번에 한다. */
	void Show();
	/** 표시를 숨기고, 아직 계산 전이면 계산도 취소한다. */
	void Hide();
private:
	/** 모든 칸을 한 번에 검사하고 텍스처에 반영한다. */
	void ComputeRange();
	/** 마스크 텍스처와 동적 머티리얼을 준비한다. */
	void PrepareMask();
	/** 칸 하나가 이동 가능한지 검사한다. MoveComponent의 이동 판정과 같은 기준. */
	bool IsCellReachable(int32 X, int32 Y) const;
	/** 칸 결과(Reachable)로 텍스처 픽셀(MaskPixels)을 다시 만든다. */
	void RebuildMaskPixels();
	/** 칸 결과를 (U, V) 위치에서 부드럽게 보간한 값 (0 ~ 1). */
	float SampleReachable(float U, float V) const;
	/** MaskPixels를 텍스처에 올린다. */
	void UploadMask();

	// 변수
private:
	/** 경로를 검사하는 칸 수 (가로, 세로). 클수록 장애물 경계가 정확하지만 경로 탐색이 늘어난다. */
	UPROPERTY(EditDefaultsOnly, Category = "MoveRange", meta = (ClampMin = "8", ClampMax = "128"))
	int32 GridResolution = 48;
	/** 표시용 텍스처 크기 (가로, 세로). 클수록 경계가 매끄럽다. 경로 탐색 횟수와는 무관하다. */
	UPROPERTY(EditDefaultsOnly, Category = "MoveRange", meta = (ClampMin = "32", ClampMax = "1024"))
	int32 TextureResolution = 256;
	/** 장애물 경계 모서리를 둥글게 만드는 정도 (칸 단위). 0이면 보간만, 클수록 둥글지만 좁은 틈이 메워질 수 있다. */
	UPROPERTY(EditDefaultsOnly, Category = "MoveRange", meta = (ClampMin = "0.0", ClampMax = "2.0"))
	float EdgeRoundness = 0.6f;
	/**
	 * Show 후 계산을 시작하기까지 기다리는 시간.
	 * 목적지를 고르기 시작할 때 자기 캡슐의 내비게이션 장애물을 끄는데, 내비메시에 반영되기까지 몇 프레임 걸린다.
	 * 그 전에 계산하면 자기 장애물 안에서 출발하게 되어 전부 이동 불가로 나온다.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "MoveRange", meta = (ClampMin = "0.0"))
	float StartDelay = 0.1f;
	/** 바닥까지 투영하는 깊이(절반 길이). 캐릭터 중심(바닥에서 약 88 위)에서 바닥에 닿아야 한다. */
	UPROPERTY(EditDefaultsOnly, Category = "MoveRange", meta = (ClampMin = "1.0"))
	float ProjectionDepth = 200.f;
	/** 데칼 머티리얼에서 마스크 텍스처를 받는 Texture Parameter 이름. */
	UPROPERTY(EditDefaultsOnly, Category = "MoveRange")
	FName MaskParameterName = TEXT("RangeMask");

	/** 코드에서 만드는 마스크 텍스처 (TextureResolution x TextureResolution). */
	UPROPERTY(Transient)
	TObjectPtr<UTexture2D> MaskTexture;
	/** 데칼 머티리얼의 동적 인스턴스. MaskTexture를 파라미터로 넘긴다. */
	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> MaskMaterial;

	/** 칸마다 이동 가능 여부 (1 = 가능, 0 = 불가). */
	TArray<float> Reachable;
	/** 텍스처 픽셀 (B, G, R, A 순서 4바이트씩). */
	TArray<uint8> MaskPixels;
	/** 계산 기준이 되는 중심 위치 (Show 시점의 캐릭터 위치). */
	FVector Center = FVector::ZeroVector;
	/** 이동 거리 = 범위 반지름 (Show 시점의 최대 이동 속도). */
	float Radius = 0.f;
	/** StartDelay 뒤 ComputeRange를 부르는 타이머. */
	FTimerHandle ComputeTimer;
};
