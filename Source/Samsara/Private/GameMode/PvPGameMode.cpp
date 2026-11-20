// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PvPGameMode.h"

#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"

#include "PlayerState/PvPPlayerState.h"
#include "PlayerController/PvPPlayerController.h"
#include "GameInstance/SamsaraGameInstance.h"
#include "Character/BaseCharacter.h"
#include "LogTest.h"
#include "Actor/PvPPlayerStart.h"

AActor* APvPGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
    Super::ChoosePlayerStart_Implementation(Player);
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(this, APvPPlayerStart::StaticClass(), PlayerStarts);
    for (auto& iter : PlayerStarts)
    {
        APvPPlayerStart* PlayerStart = Cast<APvPPlayerStart>(iter);
        if (!PlayerStart->IsSpawn)
        {
            PlayerStart->IsSpawn = true;
            APvPPlayerController* PC = Cast<APvPPlayerController>(Player);
            PC->SetTeamId(PlayerStart->GetTeamId());
            return PlayerStart;
        }
    }
    return nullptr;
}

bool APvPGameMode::ReadyToStartMatch_Implementation()
{
    if (NumPlayers != 2 || NumTravellingPlayers != 0)
        return false;

    for (APlayerState* PS : GameState->PlayerArray)
    {
        const APvPPlayerState* PvPPS = Cast<APvPPlayerState>(PS);
        if (!PvPPS || PvPPS->GetArhatNames().Num() == 0)
            return false;
    }
    return true;
}

void APvPGameMode::HandleMatchHasStarted()
{
    Super::HandleMatchHasStarted();
 
    const USamsaraGameInstance* GI = GetGameInstance<USamsaraGameInstance>();
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APvPPlayerController* PC = Cast<APvPPlayerController>(It->Get());
        const APvPPlayerState* PS = PC->GetPlayerState<APvPPlayerState>();

        FTransform SpawnTransform;
        TArray<AActor*> PlayerStarts;
        int32 TeamId = -1;
        UGameplayStatics::GetAllActorsOfClass(this, APvPPlayerStart::StaticClass(), PlayerStarts);
        for (auto& iter : PlayerStarts)
        {
            APvPPlayerStart* PlayerStart = Cast<APvPPlayerStart>(iter);
            if (PC->IsAlly(PlayerStart))
            {
                SpawnTransform = PlayerStart->GetActorTransform();
                TeamId = PlayerStart->GetTeamId();
            }
        }


        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = PC;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        UWorld* World = GetWorld();
        TArray<ABaseCharacter*> Arhats;

        for (auto& iter : PS->GetArhatNames())
        {
            ABaseCharacter* Arhat = World->SpawnActor<ABaseCharacter>(GI->GetArhatClass(iter), 
                SpawnTransform, SpawnParams);
            Arhat->SetTeamId(TeamId);
            Arhats.Add(Arhat);
        }
        PC->ServerSetArhats(Arhats);
    }
}
