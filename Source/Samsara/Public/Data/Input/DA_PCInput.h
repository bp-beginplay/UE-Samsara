// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Input/DA_BaseInput.h"
#include "DA_PCInput.generated.h"

/** 플레이어 컨트롤러에서 쓰는 입력 매핑과 입력 액션. (숫자키로 아라한 선택) */
UCLASS()
class SAMSARA_API UDA_PCInput : public UDA_BaseInput
{
	GENERATED_BODY()

	// 커스텀 함수
public:
	/** 로컬 플레이어에 매핑 컨텍스트를 추가한다. 이미 있으면 다시 추가하지 않는다. */
	void AddMapping(const ULocalPlayer* LocalPlayer) const;
	/** 로컬 플레이어에서 매핑 컨텍스트를 뺀다. */
	void RemoveMapping(const ULocalPlayer* LocalPlayer) const;

	// 변수
private:
	/** 숫자키를 아라한 선택 액션에 연결한 매핑 컨텍스트. (IMC_PC) */
	UPROPERTY(EditDefaultsOnly, Category = "Mapping")
	UInputMappingContext* MappingContext;
	/** 매핑 컨텍스트 우선순위. */
	UPROPERTY(EditDefaultsOnly, Category = "Mapping")
	int32 MappingPriority;

	/** 1 : 첫 번째 아라한 선택 */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	UInputAction* IA_SelectArhat1;
	/** 2 : 두 번째 아라한 선택 */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	UInputAction* IA_SelectArhat2;

	// Get/Set 함수
public:
	const UInputAction* GetSelectArhat1Action() const { return IA_SelectArhat1; }
	const UInputAction* GetSelectArhat2Action() const { return IA_SelectArhat2; }
};
