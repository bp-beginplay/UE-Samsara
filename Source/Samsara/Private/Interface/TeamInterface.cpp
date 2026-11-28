// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/TeamInterface.h"

bool ITeamInterface::IsAlly(const AActor* Other) const
{
	const ITeamInterface* OtherTeam = Cast<ITeamInterface>(Other);
	return OtherTeam
		&& GetTeam() != ETeam::None
		&& GetTeam() == OtherTeam->GetTeam();
}
