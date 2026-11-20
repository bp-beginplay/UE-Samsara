// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/RangeComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "Interface/TeamInterface.h"

URangeComponent::URangeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Config/DefaultEngine.ini에 정의한 "Range" 프리셋.
	// 오브젝트 타입은 Range, Pawn만 Overlap, 나머지는 모두 Ignore, 기본 상태는 NoCollision이다.
	// 프리셋에 NoCollision이 들어 있으므로 SetCollisionEnabled를 따로 부르지 않는다.
	// (프리셋 적용 뒤에 콜리전 설정을 바꾸면 프리셋 이름이 Custom으로 바뀐다.)
	SetCollisionProfileName(TEXT("Range"));
}

void URangeComponent::BeginPlay()
{
	Super::BeginPlay();

	// 블루프린트에서 콜리전을 바꿔 두었더라도 게임에서는 항상 Range 프리셋을 쓴다.
	//SetCollisionProfileName(TEXT("Range"));

	OnComponentBeginOverlap.AddDynamic(this, &URangeComponent::BeginOverlap);
}

void URangeComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 연결 이펙트는 SpawnSystemAtLocation으로 월드에 따로 만들어져 소유 액터와 함께 파괴되지 않는다.
	// 고스트처럼 소유 액터가 Destroy될 때 빔이 남지 않도록 여기서 지운다.
	ClearTargetLinkEffects();

	Super::EndPlay(EndPlayReason);
}

const ITeamInterface* URangeComponent::GetTeamSource() const
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return nullptr;
	}

	if (const ITeamInterface* InstigatorTeam = Cast<ITeamInterface>(Owner->GetInstigator()))
	{
		return InstigatorTeam;
	}
	return Cast<ITeamInterface>(Owner);
}

void URangeComponent::Active()
{
	SetSphereRadius(RangeRadius);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void URangeComponent::DeActive()
{
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ClearTargetLinkEffects();
}

void URangeComponent::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const ITeamInterface* Team = GetTeamSource();
	if (Team && Team->IsEnemy(OtherActor))
	{
		SpawnTargetLinkEffect(OtherActor);
	}
}

void URangeComponent::SpawnTargetLinkEffect(const AActor* Target)
{
	const AActor* Owner = GetOwner();
	if (!TargetLinkEffect || !Owner || !IsValid(Target))
	{
		return;
	}

	// 직접 지울 것이므로 자동 파괴는 끄고, 파라미터를 넣은 뒤에 활성화한다.
	UNiagaraComponent* Effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this, TargetLinkEffect, Owner->GetActorLocation(), FRotator::ZeroRotator, FVector(1.f),
		/*bAutoDestroy*/ false, /*bAutoActivate*/ false);
	if (!Effect)
	{
		return;
	}

	// 나이아가라 시스템의 User 파라미터(User.BeamStart, User.BeamEnd)에 월드 좌표를 넣는다.
	Effect->SetVariableVec3(TEXT("BeamStart"), Owner->GetActorLocation());
	Effect->SetVariableVec3(TEXT("BeamEnd"), Target->GetActorLocation());
	Effect->Activate();

	TargetLinkEffects.Add(Effect);
}

void URangeComponent::ClearTargetLinkEffects()
{
	for (UNiagaraComponent* Effect : TargetLinkEffects)
	{
		if (IsValid(Effect))
		{
			Effect->DestroyComponent();
		}
	}
	TargetLinkEffects.Empty();
}
