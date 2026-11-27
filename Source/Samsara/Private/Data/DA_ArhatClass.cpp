// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DA_ArhatClass.h"

TSubclassOf<ABaseCharacter> UDA_ArhatClass::FindArhatClass(const FGameplayTag& ArhatTag) const
{
	for (const FArhatClassEntry& Entry : ArhatClasses)
	{
		if (Entry.CharacterTag.MatchesTagExact(ArhatTag))
		{
			return Entry.ArhatClass;
		}
	}
	return nullptr;
}
