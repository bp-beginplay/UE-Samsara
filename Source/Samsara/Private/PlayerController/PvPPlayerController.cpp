// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/PvPPlayerController.h"

#include "Net/UnrealNetwork.h"

#include "Character/BaseCharacter.h"
#include "Data/Input/DA_PCInput.h"
#include "HUD/PvPHUD.h"

// ======================================================================
// 기본 함수
// ======================================================================

void APvPPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APvPPlayerController, Arhats);
}

void APvPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputData)
	{
		return;
	}

	InputData->AddMapping(GetLocalPlayer());
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		InputBinding::BindStarted(Input, InputData->GetSelectArhat1Action(), this, &APvPPlayerController::OnSelectArhat1);
		InputBinding::BindStarted(Input, InputData->GetSelectArhat2Action(), this, &APvPPlayerController::OnSelectArhat2);
	}
}

// ======================================================================
// 빙의
// ======================================================================

void APvPPlayerController::ServerPossessArhat_Implementation(int32 ArhatIndex)
{
	// 범위 밖 인덱스(빈 버튼, 조작된 요청)와 아직 없는 아라한은 무시한다.
	if (!Arhats.IsValidIndex(ArhatIndex) || !IsValid(Arhats[ArhatIndex]))
	{
		return;
	}

	Possess(Arhats[ArhatIndex]);
}

void APvPPlayerController::OnSelectArhat1()
{
	ServerPossessArhat(0);
}

void APvPPlayerController::OnSelectArhat2()
{
	ServerPossessArhat(1);
}

// ======================================================================
// 아라한 목록
// ======================================================================

void APvPPlayerController::ServerSetArhats_Implementation(const TArray<ABaseCharacter*>& InArhats)
{
	Arhats = InArhats;
	ServerPossessArhat(0);

	// 리슨 서버의 호스트는 OnRep이 불리지 않으므로 여기서 메뉴를 초기화한다.
	InitArhatsMenu();
}

void APvPPlayerController::OnRep_Arhats()
{
	// 배열이 캐릭터 액터보다 먼저 복제되면 아직 받지 못한 자리가 null이다.
	// 모두 도착하면 OnRep이 다시 불리므로 그때 초기화한다.
	for (const ABaseCharacter* Arhat : Arhats)
	{
		if (!IsValid(Arhat))
		{
			return;
		}
	}

	InitArhatsMenu();
}

void APvPPlayerController::InitArhatsMenu()
{
	if (APvPHUD* HUD = GetHUD<APvPHUD>())
	{
		HUD->InitMainMenu(Arhats);
	}
}
