// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MoveComponent.h"

#include "GameFramework/Pawn.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "PlayerController/PvPPlayerController.h"
#include "Character/BaseCharacter.h"
#include "LogTest.h"
#include "Actor/GhostActor.h"

UMoveComponent::UMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	OnTickFunc.ExecuteIfBound();
}

void UMoveComponent::StartSetTarget()
{
	ABaseCharacter* BaseCharacter = GetOwner<ABaseCharacter>();
	APvPPlayerController* PC = Cast<APvPPlayerController>(BaseCharacter->GetController());

	if (!IsValid(GhostActor))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = PC;
		SpawnParams.Instigator = BaseCharacter;
		GhostActor = GetWorld()->SpawnActor<AGhostActor>(GhostActorClass, SpawnParams);
		GhostActor->CopyMeshFrom(BaseCharacter->GetMesh());
	}

	BaseCharacter->GetCapsuleComponent()->SetCanEverAffectNavigation(false);

	SetComponentTickEnabled(true);
	

	OnTickFunc.BindUObject(this, &UMoveComponent::UpdateMovePreview);
}

bool UMoveComponent::SetTarget()
{
	if (IsValid(GhostActor))
	{
		SetComponentTickEnabled(false);
		OnTickFunc.Unbind();
		bCanMove = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool UMoveComponent::Active()
{
	if (bCanMove)
	{
		DestroyGhostActor();
		SetComponentTickEnabled(true);
		CurrentPointIndex = 1;
		OnTickFunc.BindUObject(this, &UMoveComponent::Move);
		return true;
	}
	else
	{
		return false;
	}

}

void UMoveComponent::Move()
{
	APawn* Pawn = GetOwner<APawn>();
	const FVector PawnLocation = Pawn->GetActorLocation();
	const FVector Target = PathPoints[CurrentPointIndex];
	if (FVector::Dist2D(PawnLocation, Target) <= AcceptanceRadius)
	{
		++CurrentPointIndex;
		if (CurrentPointIndex >= PathPoints.Num())
		{
			FinishMove();
			return;
		}
	}

	const FVector Direction = (PathPoints[CurrentPointIndex] - PawnLocation).GetSafeNormal2D();
	Pawn->AddMovementInput(Direction);
}

void UMoveComponent::FinishMove()
{
	SetComponentTickEnabled(false);
	OnTickFunc.Unbind();
	bCanMove = false;
	DestroyPathEffects();
	PathPoints.Empty();
	CurrentPointIndex = 0;
	if (ABaseCharacter* Character = GetOwner<ABaseCharacter>())
		Character->GetCapsuleComponent()->SetCanEverAffectNavigation(true);
}

void UMoveComponent::UpdateMovePreview()
{
	ABaseCharacter* BaseCharacter = GetOwner<ABaseCharacter>();
	APvPPlayerController* PC = Cast<APvPPlayerController>(BaseCharacter->GetController());
	if (!(PC && PC->IsLocalController()))
		return;
	
	const FHitResult Hit = PC->GetUnderCursorHit();
	UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(
		this, BaseCharacter->GetActorLocation(), Hit.Location, BaseCharacter);
	PathPoints = Path->PathPoints;

	if (PathPoints.IsEmpty())
		return;

	// 경로가 이동 가능 거리(MaxSpeed)보다 길면, 경로를 따라 정확히 MaxSpeed 지점에서 자른다.
	// 그러면 마지막 지점은 커서(Hit.Location) 쪽으로 갈 수 있는 가장 먼 곳이 되고, 총길이는 MaxSpeed가 된다.
	const float MaxLength = BaseCharacter->GetCharacterMovement()->GetMaxSpeed();
	float Length = 0.f;
	for (int32 i = 1; i < PathPoints.Num(); ++i)
	{
		const float SegmentDist = FVector::Dist(PathPoints[i - 1], PathPoints[i]);
		if (Length + SegmentDist > MaxLength)
		{
			// 이 구간 안에서 남은 거리만큼만 간 위치를 마지막 지점으로 하고, 그 뒤 지점은 버린다.
			const float Alpha = (MaxLength - Length) / SegmentDist;
			PathPoints[i] = FMath::Lerp(PathPoints[i - 1], PathPoints[i], Alpha);
			PathPoints.SetNum(i + 1);
			break;
		}
		Length += SegmentDist;
	}

	if(IsValid(GhostActor))
		GhostActor->SetActorLocation(PathPoints.Last());

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

void UMoveComponent::DestroyGhostActor()
{
	if (IsValid(GhostActor))
		GhostActor->Destroy();
}

void UMoveComponent::DestroyPathEffects()
{
	while (!PathEffects.IsEmpty())
	{
		UNiagaraComponent* Effect = PathEffects.Pop();
		if (IsValid(Effect))
		{
			Effect->DestroyComponent();
		}
	}
}
