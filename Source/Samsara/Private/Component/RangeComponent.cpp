// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/RangeComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include "Character/BaseCharacter.h"

// ======================================================================
// 기본 함수
// ======================================================================

URangeComponent::URangeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	// Config/DefaultEngine.ini에 정의한 "Range" 프리셋 (Pawn만 Overlap, 서로는 무시, 기본 NoCollision)
	SetCollisionProfileName(TEXT("Range"));
	bHiddenInGame = false;
}

void URangeComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &URangeComponent::OnRangeBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &URangeComponent::OnRangeEndOverlap);
}

void URangeComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 이펙트는 SpawnSystemAtLocation으로 월드에 따로 만들어져 소유 액터와 함께 파괴되지 않는다.
	ClearTargetLinkEffects();

	Super::EndPlay(EndPlayReason);
}

void URangeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateTargetLinkEffects();
}

// ======================================================================
// 감지 켜기 / 끄기
// ======================================================================

void URangeComponent::EnableDetection()
{
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void URangeComponent::SetRangeRadius(float InRadius)
{
	SetSphereRadius(InRadius);
}

// ======================================================================
// 범위 안 캐릭터 관리
// ======================================================================

void URangeComponent::OnRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AddTarget(Cast<ABaseCharacter>(OtherActor));
}

void URangeComponent::OnRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	RemoveTarget(Cast<ABaseCharacter>(OtherActor));
}

void URangeComponent::AddTarget(ABaseCharacter* Target)
{
	const AActor* Owner = GetOwner();
	if (!Target || !Owner || TargetsInRange.Contains(Target))
	{
		return;
	}

	// 위치는 매 틱 UpdateTargetLinkEffects에서 맞추고 그때 활성화한다.
	UNiagaraComponent* Effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this, TargetLinkEffectSystem, Owner->GetActorLocation(), FRotator::ZeroRotator, FVector(1.f),
		/*bAutoDestroy*/ false, /*bAutoActivate*/ false);

	// 이펙트가 만들어지지 않아도 인덱스 짝을 맞추기 위해 함께 넣는다.
	TargetsInRange.Add(Target);
	TargetLinkEffectComponents.Add(Effect);
}

void URangeComponent::RemoveTarget(ABaseCharacter* Target)
{
	const int32 Index = TargetsInRange.Find(Target);
	if (Index == INDEX_NONE)
	{
		return;
	}

	// 나간 캐릭터와 짝인 이펙트를 지운다.
	if (TargetLinkEffectComponents.IsValidIndex(Index))
	{
		if (UNiagaraComponent* Effect = TargetLinkEffectComponents[Index])
		{
			Effect->DestroyComponent();
		}
		TargetLinkEffectComponents.RemoveAt(Index);
	}
	TargetsInRange.RemoveAt(Index);
}

void URangeComponent::UpdateTargetLinkEffects()
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	const int32 Count = FMath::Min(TargetsInRange.Num(), TargetLinkEffectComponents.Num());
	for (int32 i = 0; i < Count; ++i)
	{
		UNiagaraComponent* Effect = TargetLinkEffectComponents[i];
		const ABaseCharacter* Target = TargetsInRange[i];
		if (!IsValid(Effect) || !IsValid(Target))
		{
			continue;
		}

		// 나이아가라 시스템의 User 파라미터(User.BeamStart, User.BeamEnd)에 월드 좌표를 넣는다.
		Effect->SetVariableVec3(TEXT("BeamStart"), Owner->GetActorLocation());
		Effect->SetVariableVec3(TEXT("BeamEnd"), Target->GetActorLocation());
		Effect->Activate();
	}
}

void URangeComponent::ClearTargetLinkEffects()
{
	for (UNiagaraComponent* Effect : TargetLinkEffectComponents)
	{
		if (IsValid(Effect))
		{
			Effect->DestroyComponent();
		}
	}
	TargetLinkEffectComponents.Empty();
	TargetsInRange.Empty();
}
