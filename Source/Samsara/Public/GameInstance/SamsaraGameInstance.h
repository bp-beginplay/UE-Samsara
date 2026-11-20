// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"

#include "SamsaraGameInstance.generated.h"

class UDA_ArhatClass;
class ABaseCharacter;

UCLASS()
class SAMSARA_API USamsaraGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
	UDA_ArhatClass* DA_ArhatClass;

public:
	TSubclassOf<ABaseCharacter> GetArhatClass(const FGameplayTag& InTag) const;
};
