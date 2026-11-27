// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Input/DA_BaseInput.h"
#include "DA_ArhatInput.generated.h"

class AController;

/** 아라한(캐릭터)을 조종할 때 쓰는 입력 매핑과 입력 액션. */
UCLASS()
class SAMSARA_API UDA_ArhatInput : public UDA_BaseInput
{
	GENERATED_BODY()

	// 커스텀 함수
public:
	/** 컨트롤러의 로컬 플레이어에 매핑 컨텍스트를 추가한다. */
	void AddMapping(const AController* Controller) const;
	void RemoveMapping(const AController* Controller) const;

	// 변수
private:
	UPROPERTY(EditDefaultsOnly, Category = "Mapping")
	UInputMappingContext* MappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Mapping")
	int32 MappingPriority;

	/** 마우스 이동 : 카메라 이동 */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	UInputAction* IA_CameraMoveMouse;
	/** S : 목적지 고르기 시작 */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	UInputAction* IA_SelectDestination;
	/** 좌클릭 : 목적지 확정 */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	UInputAction* IA_ConfirmDestination;
	/** 스페이스 : 이동 시작 */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	UInputAction* IA_StartMove;
	/** A : 공격 사거리 표시 */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	UInputAction* IA_ShowAttackRange;

	// Get/Set 함수
public:
	const UInputAction* GetCameraMoveMouseAction() const { return IA_CameraMoveMouse; }
	const UInputAction* GetSelectDestinationAction() const { return IA_SelectDestination; }
	const UInputAction* GetConfirmDestinationAction() const { return IA_ConfirmDestination; }
	const UInputAction* GetStartMoveAction() const { return IA_StartMove; }
	const UInputAction* GetShowAttackRangeAction() const { return IA_ShowAttackRange; }
};
