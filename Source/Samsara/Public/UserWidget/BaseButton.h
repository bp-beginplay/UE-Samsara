// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseButton.generated.h"

class UButton;
class UTexture2D;

/** 이미지 버튼 위젯의 공통 클래스. */
UCLASS()
class SAMSARA_API UBaseButton : public UUserWidget
{
	GENERATED_BODY()

	// 커스텀 함수
public:
	/** 버튼의 기본, 호버, 눌림 상태 이미지를 모두 바꾼다. */
	void SetButtonImage(UTexture2D* InImage);

	// 변수
private:
	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	// Get/Set 함수
public:
	UButton* GetButton() const { return Button; }
};
