// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostActor.generated.h"

class USkeletalMeshComponent;
class UMaterialInterface;
class URangeComponent;

/**
 * 이동 목적지에 캐릭터와 같은 메시로 표시하는 미리보기 액터.
 * 목적지를 정한 뒤에는 공격·스킬 사거리의 기준점이 된다.
 * 조종하는 플레이어 화면에만 필요하므로 복제하지 않는다.
 */
UCLASS()
class SAMSARA_API AGhostActor : public AActor
{
	GENERATED_BODY()

	// 기본 함수
public:
	/** 루트, 충돌 없는 고스트 메시, 사거리 컴포넌트를 만든다. */
	AGhostActor();
protected:
	/** 사거리 감지를 켠다. */
	virtual void BeginPlay() override;

	// 커스텀 함수
public:
	/** 원본 캐릭터의 메시를 복사해 고스트에 적용한다. 스폰 직후 호출한다. */
	void CopyMeshFrom(const USkeletalMeshComponent* SourceMesh);
	/** 고스트 위치 기준 사거리를 바꾼다. */
	void SetRangeRadius(float InRadius);

	// 변수
private:
	/** 루트. 이 위치가 목적지(바닥 지점)다. */
	UPROPERTY(VisibleAnywhere, Category = "Ghost")
	TObjectPtr<USceneComponent> Root;
	/** 캐릭터 메시를 복사해 보여 주는 메시. 충돌, 그림자, 데칼을 모두 끈다. */
	UPROPERTY(VisibleAnywhere, Category = "Ghost")
	TObjectPtr<USkeletalMeshComponent> GhostMesh;
	/** 고스트에 덮어쓸 머티리얼. 비워 두면 캐릭터 원래 머티리얼을 그대로 쓴다. */
	UPROPERTY(EditDefaultsOnly, Category = "Ghost")
	TObjectPtr<UMaterialInterface> GhostMaterial;

	/** 목적지 기준 공격 사거리. 이동한 뒤 닿는 캐릭터를 미리 보여 준다. */
	UPROPERTY(VisibleAnywhere, Category = "Range")
	TObjectPtr<URangeComponent> RangeComponent;
};
