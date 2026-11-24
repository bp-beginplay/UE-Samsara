// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/RangeComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "LogTest.h"
#include "Character/BaseCharacter.h"

URangeComponent::URangeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	SetCollisionProfileName(TEXT("Range"));
}

void URangeComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &URangeComponent::BeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &URangeComponent::EndOverlap);
}

void URangeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Arhats.IsEmpty())
		return;

	const AActor* Owner = GetOwner();
	for (int i = 0; i < Arhats.Num(); i++)
	{
		TargetLinkEffects[i]->SetVariableVec3(TEXT("BeamStart"), Owner->GetActorLocation());
		TargetLinkEffects[i]->SetVariableVec3(TEXT("BeamEnd"), Arhats[i]->GetActorLocation());
		TargetLinkEffects[i]->Activate();
	}
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
	const AActor* Owner = GetOwner();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor))
	{
		Arhats.AddUnique(BaseCharacter);
		UNiagaraComponent* Effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this, TargetLinkEffect, Owner->GetActorLocation(), FRotator::ZeroRotator, FVector(1.f), false, false);
		TargetLinkEffects.AddUnique(Effect);
	}
}

void URangeComponent::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor))
	{
		Arhats.Remove(BaseCharacter);
		UNiagaraComponent* Effect = TargetLinkEffects.Pop();
		if (IsValid(Effect))
		{
			Effect->DestroyComponent();
		}
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
