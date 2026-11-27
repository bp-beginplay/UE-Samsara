// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArhatsBox.generated.h"

class UArhatButton;
class ABaseCharacter;

/** 아라한 선택 버튼 6개를 담은 위젯. 버튼을 누르면 그 아라한에 빙의한다. */
UCLASS()
class SAMSARA_API UArhatsBox : public UUserWidget
{
	GENERATED_BODY()

	// 기본 함수
protected:
	virtual void NativeOnInitialized() override;

	// 커스텀 함수
public:
	/** 아라한 순서대로 버튼에 아이콘을 넣는다. */
	void InitArhatButtons(const TArray<ABaseCharacter*>& Arhats);

private:
	/** 버튼 6개를 순서대로 모은 배열. */
	TArray<UArhatButton*> GetArhatButtons() const;
	/** ArhatIndex번 아라한에 빙의를 요청한다. */
	void RequestPossessArhat(int32 ArhatIndex);

	// 버튼 클릭 (OnClicked에 바인딩하려면 버튼마다 함수가 필요하다)
	UFUNCTION()
	void ClickArhatButton1();
	UFUNCTION()
	void ClickArhatButton2();
	UFUNCTION()
	void ClickArhatButton3();
	UFUNCTION()
	void ClickArhatButton4();
	UFUNCTION()
	void ClickArhatButton5();
	UFUNCTION()
	void ClickArhatButton6();

	// 변수
private:
	UPROPERTY(meta = (BindWidget))
	UArhatButton* ArhatButton1;
	UPROPERTY(meta = (BindWidget))
	UArhatButton* ArhatButton2;
	UPROPERTY(meta = (BindWidget))
	UArhatButton* ArhatButton3;
	UPROPERTY(meta = (BindWidget))
	UArhatButton* ArhatButton4;
	UPROPERTY(meta = (BindWidget))
	UArhatButton* ArhatButton5;
	UPROPERTY(meta = (BindWidget))
	UArhatButton* ArhatButton6;
};
