// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "DA_ArhatClass.generated.h"

class ABaseCharacter;

USTRUCT(BlueprintType)
struct FArhatClass
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CharacterTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ABaseCharacter> ArhatClass;
};

UCLASS()
class SAMSARA_API UDA_ArhatClass : public UDataAsset
{
	GENERATED_BODY()

public:
	TSubclassOf<ABaseCharacter> GetArhatClassByTag(const FGameplayTag& InTag) const;

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FArhatClass> ArhatClasses;
};
