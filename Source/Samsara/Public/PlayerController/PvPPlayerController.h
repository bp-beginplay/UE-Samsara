// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/BasePlayerController.h"

#include "Interface/TeamInterface.h"

#include "PvPPlayerController.generated.h"

class ABaseCharacter;
class UDA_PCInput;

UCLASS()
class SAMSARA_API APvPPlayerController : public ABasePlayerController, public ITeamInterface
{
	GENERATED_BODY()
	

	// 기본 함수
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupInputComponent() override;

	// 커스텀 함수
public:
	UFUNCTION(Reliable, Server)
	void ServerPossess(int _Index);
private:
	void Possess1();
	void Possess2();


private:
	int32 TeamId = INDEX_NONE;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Arhats)
	TArray<ABaseCharacter*> Arhats;
	UPROPERTY(EditDefaultsOnly)
	UDA_PCInput* DA_PCInput;

	// Set/Get함수
public:
	UFUNCTION(Reliable, Server)
	void ServerSetArhats(const TArray<ABaseCharacter*>& _Arhats);
	virtual void SetTeamId(int32 NewTeamId) override { TeamId = NewTeamId; }

	virtual int32 GetTeamId() const override { return TeamId; }

	// OnRep함수
private:
	UFUNCTION()
	void OnRep_Arhats();
};