// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidget/BaseButton.h"
#include "ArhatButton.generated.h"

/** 아라한 선택 버튼. 아라한 아이콘을 보여 주고, 누르면 ArhatsBox가 그 아라한에 빙의를 요청한다. */
UCLASS()
class SAMSARA_API UArhatButton : public UBaseButton
{
	GENERATED_BODY()
};
