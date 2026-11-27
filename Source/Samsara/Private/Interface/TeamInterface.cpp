// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/TeamInterface.h"

bool ITeamInterface::IsAlly(const AActor* Other) const
{
	const ITeamInterface* OtherTeam = Cast<ITeamInterface>(Other);
	return OtherTeam
		&& GetTeamId() != INDEX_NONE
		&& GetTeamId() == OtherTeam->GetTeamId();
}

bool ITeamInterface::IsEnemy(const AActor* Other) const
{
	const ITeamInterface* OtherTeam = Cast<ITeamInterface>(Other);
	return OtherTeam
		&& GetTeamId() != INDEX_NONE
		&& OtherTeam->GetTeamId() != INDEX_NONE
		&& GetTeamId() != OtherTeam->GetTeamId();
}
