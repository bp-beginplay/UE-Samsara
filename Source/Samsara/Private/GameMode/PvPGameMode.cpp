// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PvPGameMode.h"

#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"

#include "Actor/PvPPlayerStart.h"
#include "Character/BaseCharacter.h"
#include "GameInstance/SamsaraGameInstance.h"
#include "PlayerController/PvPPlayerController.h"
#include "PlayerState/PvPPlayerState.h"

// ======================================================================
// 기본 함수
// ======================================================================

AActor* APvPGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	Super::ChoosePlayerStart_Implementation(Player);

	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APvPPlayerStart::StaticClass(), PlayerStarts);
	for (AActor* Actor : PlayerStarts)
	{
		APvPPlayerStart* PlayerStart = Cast<APvPPlayerStart>(Actor);
		if (!PlayerStart || PlayerStart->IsSpawned())
		{
			continue;
		}

		PlayerStart->SetSpawned(true);
		if (APvPPlayerController* PC = Cast<APvPPlayerController>(Player))
		{
			PC->SetTeam(PlayerStart->GetTeam());
		}
		return PlayerStart;
	}
	return nullptr;
}

bool APvPGameMode::ReadyToStartMatch_Implementation()
{
	if (NumPlayers != 2 || NumTravellingPlayers != 0)
	{
		return false;
	}

	for (APlayerState* PS : GameState->PlayerArray)
	{
		const APvPPlayerState* PvPPS = Cast<APvPPlayerState>(PS);
		if (!PvPPS || PvPPS->GetArhatTags().Num() == 0)
		{
			return false;
		}
	}
	return true;
}

void APvPGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APvPPlayerController* PC = Cast<APvPPlayerController>(It->Get());
		if (!PC)
		{
			continue;
		}

		const APvPPlayerStart* PlayerStart = FindTeamPlayerStart(PC);
		PC->ServerSetArhats(SpawnArhats(PC, PlayerStart));
	}
}

// ======================================================================
// 아라한 스폰
// ======================================================================

APvPPlayerStart* APvPGameMode::FindTeamPlayerStart(const APvPPlayerController* PC) const
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APvPPlayerStart::StaticClass(), PlayerStarts);
	for (AActor* Actor : PlayerStarts)
	{
		if (PC->IsAlly(Actor))
		{
			return Cast<APvPPlayerStart>(Actor);
		}
	}
	return nullptr;
}

TArray<ABaseCharacter*> APvPGameMode::SpawnArhats(APvPPlayerController* PC, const APvPPlayerStart* PlayerStart) const
{
	TArray<ABaseCharacter*> Arhats;

	const USamsaraGameInstance* GI = GetGameInstance<USamsaraGameInstance>();
	const APvPPlayerState* PS = PC->GetPlayerState<APvPPlayerState>();
	if (!GI || !PS)
	{
		return Arhats;
	}

	const FTransform SpawnTransform = PlayerStart ? PlayerStart->GetActorTransform() : FTransform::Identity;
	const ETeam Team = PlayerStart ? PlayerStart->GetTeam() : ETeam::None;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PC;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (const FGameplayTag& ArhatTag : PS->GetArhatTags())
	{
		ABaseCharacter* Arhat = GetWorld()->SpawnActor<ABaseCharacter>(GI->GetArhatClass(ArhatTag), SpawnTransform, SpawnParams);
		if (!Arhat)
		{
			continue;
		}
		Arhat->SetTeam(Team);
		Arhats.Add(Arhat);
	}
	return Arhats;
}
