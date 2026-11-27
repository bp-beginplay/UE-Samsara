// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnhancedInputComponent.h"

#include "DA_BaseInput.generated.h"

class UInputAction;
class UInputMappingContext;

/** 입력 매핑과 입력 액션을 담는 데이터 에셋의 공통 클래스. */
UCLASS()
class SAMSARA_API UDA_BaseInput : public UDataAsset
{
	GENERATED_BODY()
};

/** Enhanced Input 바인딩을 짧게 쓰기 위한 함수들. */
namespace InputBinding
{
	/** 키를 누른 순간 한 번 */
	template<class T, class FuncType>
	void BindStarted(UEnhancedInputComponent* Input, const UInputAction* Action, T* Object, FuncType Func)
	{
		Input->BindAction(Action, ETriggerEvent::Started, Object, Func);
	}

	/** 키를 누르고 있는 동안 매 프레임 */
	template<class T, class FuncType>
	void BindTriggered(UEnhancedInputComponent* Input, const UInputAction* Action, T* Object, FuncType Func)
	{
		Input->BindAction(Action, ETriggerEvent::Triggered, Object, Func);
	}

	/** 키를 뗀 순간 한 번 */
	template<class T, class FuncType>
	void BindCompleted(UEnhancedInputComponent* Input, const UInputAction* Action, T* Object, FuncType Func)
	{
		Input->BindAction(Action, ETriggerEvent::Completed, Object, Func);
	}

	/** 누르고 있는 동안 + 뗀 순간 */
	template<class T, class TriggeredFuncType, class CompletedFuncType>
	void BindTriggeredAndCompleted(UEnhancedInputComponent* Input, const UInputAction* Action, T* Object, TriggeredFuncType TriggeredFunc, CompletedFuncType CompletedFunc)
	{
		Input->BindAction(Action, ETriggerEvent::Triggered, Object, TriggeredFunc);
		Input->BindAction(Action, ETriggerEvent::Completed, Object, CompletedFunc);
	}
}
