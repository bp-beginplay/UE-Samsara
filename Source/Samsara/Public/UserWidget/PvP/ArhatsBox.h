// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArhatsBox.generated.h"

class UArhatButton;
class ABaseCharacter;

UCLASS()
class SAMSARA_API UArhatsBox : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

public:
	void InitArhatButtons(TArray<ABaseCharacter*> _Arhats);
private:
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
	void ClickArhat(int _Index);

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
