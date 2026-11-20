// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Input/DA_BaseInput.h"
#include "DA_PCInput.generated.h"

UCLASS()
class SAMSARA_API UDA_PCInput : public UDA_BaseInput
{
	GENERATED_BODY()
	
public:
	void AddMappingPC(const ULocalPlayer* LocalPlayer) const;
	void RemoveMappingContext(const ULocalPlayer* LocalPlayer) const;

private:
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* IMC_PC;
	UPROPERTY(EditDefaultsOnly)
	int32 Pri_PC;
public:
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_PC1;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_PC2;
};
