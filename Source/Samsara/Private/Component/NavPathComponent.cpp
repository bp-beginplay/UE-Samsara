// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/NavPathComponent.h"

#include "GameFramework/Pawn.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "PlayerController/BasePlayerController.h"
#include "Character/BaseCharacter.h"
#include "LogTest.h"

UNavPathComponent::UNavPathComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UNavPathComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ABaseCharacter* BaseCharacter = GetOwner<ABaseCharacter>();
	ABasePlayerController* PC = Cast<ABasePlayerController>(BaseCharacter->GetController());

	if (PC && PC->IsLocalController())
	{
		const FHitResult Hit = PC->GetUnderCursorHit();
		UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(
			this, BaseCharacter->GetActorLocation(), Hit.Location, BaseCharacter);
		PathPoints = Path->PathPoints;

		const int32 NumSegments = FMath::Max(PathPoints.Num() - 1, 0);

		while (PathEffects.Num() < NumSegments)
		{
			UNiagaraComponent* Effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this, PathEffect, PathPoints[PathEffects.Num()], FRotator::ZeroRotator, FVector(1.f), false, true);
			PathEffects.Add(Effect);
		}
		while (PathEffects.Num() > NumSegments)
		{
			UNiagaraComponent* Effect = PathEffects.Pop();
			if (IsValid(Effect))
			{
				Effect->DestroyComponent();
			}
		}

		const FVector Offset(0.f, 0.f, 10.f);
		for (int32 i = 0; i < PathEffects.Num(); ++i)
		{
			UNiagaraComponent* Effect = PathEffects[i];
			if (IsValid(Effect))
			{
				Effect->SetVariableVec3(TEXT("BeamStart"), PathPoints[i] + Offset);
				Effect->SetVariableVec3(TEXT("BeamEnd"), PathPoints[i + 1] + Offset);
			}
		}
	}
}

void UNavPathComponent::Active()
{
	SetComponentTickEnabled(true);
}

void UNavPathComponent::DeActive()
{

}