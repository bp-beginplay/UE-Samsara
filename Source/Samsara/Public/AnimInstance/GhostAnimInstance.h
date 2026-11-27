// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GhostAnimInstance.generated.h"

/** 고스트(GhostActor) 애니메이션 블루프린트의 부모 클래스. 지금은 C++에서 할 일이 없다. */
UCLASS()
class SAMSARA_API UGhostAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
};
