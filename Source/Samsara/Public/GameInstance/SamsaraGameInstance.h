// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "GameplayTagContainer.h"

#include "SamsaraGameInstance.generated.h"

class UDA_ArhatClass;
class ABaseCharacter;

/** 게임 전체에서 쓰는 데이터를 가진 게임 인스턴스. */
UCLASS()
class SAMSARA_API USamsaraGameInstance : public UGameInstance
{
	GENERATED_BODY()

	// 커스텀 함수
public:
	/** 태그에 해당하는 아라한 클래스. 없으면 nullptr. */
	TSubclassOf<ABaseCharacter> GetArhatClass(const FGameplayTag& ArhatTag) const;

	// 변수
private:
	/** 아라한 태그 → 아라한 클래스 표. */
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDA_ArhatClass* ArhatClassData;
};
