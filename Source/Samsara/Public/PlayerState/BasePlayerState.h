// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "GameplayTagContainer.h"

#include "BasePlayerState.generated.h"

UCLASS()
class SAMSARA_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	TArray<FGameplayTag> ArhatNames;

public:
	TArray<FGameplayTag> GetArhatNames() const { return ArhatNames; }
};
