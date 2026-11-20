// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Input/DA_BaseInput.h"
#include "DA_ArhatInput.generated.h"

class AController;

UCLASS()
class SAMSARA_API UDA_ArhatInput : public UDA_BaseInput
{
	GENERATED_BODY()
	
public:
	void AddMappingArhat(const AController* Controller) const;
	void RemoveMappingArhat(const AController* Controller) const;

private:
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* IMC_Arhat;
	UPROPERTY(EditDefaultsOnly)
	int32 Pri_Arhat;
public:
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_ArhatWASD;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_ArhatMouxeXY;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_ArhatLMB;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_ArhatSB;
};
