// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/SamsaraGameInstance.h"

#include "Data/DA_ArhatClass.h"
#include "Character/BaseCharacter.h"

TSubclassOf<ABaseCharacter> USamsaraGameInstance::GetArhatClass(const FGameplayTag& InTag) const
{
	return DA_ArhatClass->GetArhatClassByTag(InTag);
}
