// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "GameplayTagContainer.h"

#include "DA_ArhatClass.generated.h"

class ABaseCharacter;

/** 아라한 태그 하나와 그 태그로 스폰할 캐릭터 클래스. */
USTRUCT(BlueprintType)
struct FArhatClassEntry
{
	GENERATED_BODY()

	/** 아라한을 가리키는 태그. (예: Synergy.A.Aurora) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CharacterTag;
	/** 이 태그로 스폰할 아라한 블루프린트 클래스. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ABaseCharacter> ArhatClass;
};

/** 아라한 태그 → 캐릭터 클래스 표. */
UCLASS()
class SAMSARA_API UDA_ArhatClass : public UDataAsset
{
	GENERATED_BODY()

	// 커스텀 함수
public:
	/** 태그와 정확히 일치하는 아라한 클래스. 없으면 nullptr. */
	TSubclassOf<ABaseCharacter> FindArhatClass(const FGameplayTag& ArhatTag) const;

	// 변수
private:
	/** 게임에 나오는 모든 아라한의 태그와 클래스. */
	UPROPERTY(EditDefaultsOnly)
	TArray<FArhatClassEntry> ArhatClasses;
};
