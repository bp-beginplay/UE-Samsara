// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidget/BaseButton.h"
#include "SkillButton.generated.h"

/** 스킬 버튼. 지금은 WBP_SkillButton, WBP_MoveButton의 부모 클래스로만 쓰고, 기능은 스킬을 만들 때 넣는다. */
UCLASS()
class SAMSARA_API USkillButton : public UBaseButton
{
	GENERATED_BODY()
};
