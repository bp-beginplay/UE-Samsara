// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/SamsaraGameInstance.h"

#include "Character/BaseCharacter.h"
#include "Data/DA_ArhatClass.h"

TSubclassOf<ABaseCharacter> USamsaraGameInstance::GetArhatClass(const FGameplayTag& ArhatTag) const
{
	return ArhatClassData ? ArhatClassData->FindArhatClass(ArhatTag) : nullptr;
}
