// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DA_ArhatClass.h"

TSubclassOf<ABaseCharacter> UDA_ArhatClass::GetArhatClassByTag(const FGameplayTag& InTag) const
{
    for (const FArhatClass& Info : ArhatClasses)
    {
        if (Info.CharacterTag.MatchesTagExact(InTag))
        {
            return Info.ArhatClass;
        }
    }
    return nullptr;
}