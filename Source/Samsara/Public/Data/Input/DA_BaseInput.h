// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnhancedInputComponent.h"

#include "DA_BaseInput.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class SAMSARA_API UDA_BaseInput : public UDataAsset
{
	GENERATED_BODY()
};

namespace EBindInput
{
	template<class T, class StartFuncType>
	void Started(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj, StartFuncType StartFunc)
	{
		Input->BindAction(Action, ETriggerEvent::Started, Obj, StartFunc);
	}

	template<class T, class TriggerFuncType>
	void Triggered(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj, TriggerFuncType TriggerFunc)
	{
		Input->BindAction(Action, ETriggerEvent::Triggered, Obj, TriggerFunc);
	}

	template<class T, class CompleteFuncType>
	void Complete(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj, CompleteFuncType CompleteFunc)
	{
		Input->BindAction(Action, ETriggerEvent::Completed, Obj, CompleteFunc);
	}

	template<class T, class TriggerFuncType, class CompleteFuncType>
	void TriggeredComplete(UEnhancedInputComponent* Input, const UInputAction* Action, T* Obj, TriggerFuncType TriggerFunc, CompleteFuncType CompleteFunc)
	{
		Input->BindAction(Action, ETriggerEvent::Triggered, Obj, TriggerFunc);
		Input->BindAction(Action, ETriggerEvent::Completed, Obj, CompleteFunc);
	}
}