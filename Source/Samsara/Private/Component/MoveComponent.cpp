// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MoveComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include "Actor/GhostActor.h"
#include "Character/BaseCharacter.h"
#include "PlayerController/PvPPlayerController.h"

// ======================================================================
// 기본 함수
// ======================================================================

UMoveComponent::UMoveComponent()
{
	// 목적지를 고르거나 이동하는 동안에만 틱을 켠다.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TickFunction.ExecuteIfBound();
}

// ======================================================================
// 목적지 고르기 → 확정 → 이동 시작
// ======================================================================

void UMoveComponent::BeginSelectDestination()
{
	SpawnGhostActor();
	SetOwnerNavObstacle(false);

	SetComponentTickEnabled(true);
	TickFunction.BindUObject(this, &UMoveComponent::UpdateDestinationPreview);
}

bool UMoveComponent::ConfirmDestination()
{
	if (!IsValid(GhostActor) || bHasDestination)
	{
		return false;
	}

	SetComponentTickEnabled(false);
	TickFunction.Unbind();
	bHasDestination = true;
	return true;
}

bool UMoveComponent::StartMove()
{
	if (!bHasDestination)
	{
		return false;
	}

	DestroyGhostActor();

	// PathPoints[0]은 출발 위치이므로 1번 지점부터 향한다.
	TargetPointIndex = 1;
	SetComponentTickEnabled(true);
	TickFunction.BindUObject(this, &UMoveComponent::UpdateMove);
	return true;
}

// ======================================================================
// 목적지 고르기 (미리보기)
// ======================================================================

void UMoveComponent::UpdateDestinationPreview()
{
	ABaseCharacter* BaseCharacter = GetOwner<ABaseCharacter>();
	if (!BaseCharacter)
	{
		return;
	}
	APvPPlayerController* PC = Cast<APvPPlayerController>(BaseCharacter->GetController());
	if (!PC || !PC->IsLocalController())
	{
		return;
	}

	const FHitResult Hit = PC->GetUnderCursorHit();
	if (!Hit.bBlockingHit)
	{
		return;
	}

	// 커서가 도착할 수 없는 곳을 가리켜도 그 근처의 갈 수 있는 곳까지 경로를 만든다.
	// 경로를 전혀 만들 수 없으면 이전 미리보기를 그대로 둔다.
	if (!BuildPreviewPath(BaseCharacter, Hit.Location))
	{
		return;
	}

	// 한 턴에 갈 수 있는 거리까지만 남긴다.
	TrimPathToLength(BaseCharacter->GetCharacterMovement()->GetMaxSpeed());

	if (IsValid(GhostActor))
	{
		GhostActor->SetActorLocation(PathPoints.Last());
	}
	UpdatePathEffects();
}

bool UMoveComponent::BuildPreviewPath(AActor* Agent, const FVector& CursorLocation)
{
	if (!Agent)
	{
		return false;
	}
	const FVector Start = Agent->GetActorLocation();

	// Goal까지 길찾기를 해서 성공하면 PathPoints에 넣는다.
	// 엔진 기본 설정이 부분 경로를 허용하므로, Goal에 도착할 수 없으면 갈 수 있는 가장 가까운 곳까지의 경로가 나온다.
	auto TryPathTo = [&](const FVector& Goal) -> bool
	{
		UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(this, Start, Goal, Agent);
		if (Path && Path->IsValid() && Path->PathPoints.Num() >= 2)
		{
			PathPoints = Path->PathPoints;
			return true;
		}
		return false;
	};

	// 1) 커서 위치로 바로 길찾기
	//    커서가 내비메시 근처면 여기서 끝난다. (도착 불가인 섬 위, 좁은 장애물 위 등은 부분 경로로 처리됨)
	if (TryPathTo(CursorLocation))
	{
		return true;
	}

	// 2) 커서 위치를 넓은 범위에서 가장 가까운 내비메시 지점으로 옮겨 다시 길찾기
	//    큰 상자 위, 다른 캐릭터 한가운데처럼 커서가 내비메시에서 멀어 1)이 목적지를 못 찾은 경우
	if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		FNavLocation Projected;
		if (NavSys->ProjectPointToNavigation(CursorLocation, Projected, UnreachableProjectExtent) && TryPathTo(Projected.Location))
		{
			return true;
		}
	}

	// 3) 캐릭터에서 커서 방향으로 내비메시를 따라가다 막히는 지점까지
	//    커서 근처에 내비메시가 전혀 없는 경우 (아주 큰 장애물 위 등)
	const FVector RayEnd(CursorLocation.X, CursorLocation.Y, Start.Z);
	FVector BlockedLocation;
	if (UNavigationSystemV1::NavigationRaycast(this, Start, RayEnd, BlockedLocation) && TryPathTo(BlockedLocation))
	{
		return true;
	}

	return false;
}

void UMoveComponent::TrimPathToLength(float MaxLength)
{
	// 경로를 앞에서부터 따라가다가 누적 길이가 MaxLength를 넘는 구간에서,
	// 정확히 MaxLength가 되는 위치를 마지막 지점으로 하고 그 뒤 지점은 버린다.
	// 그러면 마지막 지점은 커서 쪽으로 갈 수 있는 가장 먼 곳이 되고, 총길이는 MaxLength가 된다.
	float Length = 0.f;
	for (int32 i = 1; i < PathPoints.Num(); ++i)
	{
		const float SegmentDist = FVector::Dist(PathPoints[i - 1], PathPoints[i]);
		if (Length + SegmentDist > MaxLength)
		{
			const float Alpha = (MaxLength - Length) / SegmentDist;
			PathPoints[i] = FMath::Lerp(PathPoints[i - 1], PathPoints[i], Alpha);
			PathPoints.SetNum(i + 1);
			return;
		}
		Length += SegmentDist;
	}
}

void UMoveComponent::UpdatePathEffects()
{
	// 지점이 N개면 구간은 N-1개
	const int32 NumSegments = FMath::Max(PathPoints.Num() - 1, 0);

	// 부족한 만큼 새로 만든다.
	while (PathEffectComponents.Num() < NumSegments)
	{
		UNiagaraComponent* Effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this, PathEffectSystem, PathPoints[PathEffectComponents.Num()], FRotator::ZeroRotator, FVector(1.f),
			/*bAutoDestroy*/ false, /*bAutoActivate*/ true);
		PathEffectComponents.Add(Effect);
	}

	// 남는 만큼 지운다. Pop으로 배열이 줄어야 반복이 끝난다.
	while (PathEffectComponents.Num() > NumSegments)
	{
		UNiagaraComponent* Effect = PathEffectComponents.Pop();
		if (IsValid(Effect))
		{
			Effect->DestroyComponent();
		}
	}

	// 구간 i는 PathPoints[i] → PathPoints[i + 1]을 잇는다.
	const FVector Offset(0.f, 0.f, PathEffectHeightOffset);
	for (int32 i = 0; i < NumSegments; ++i)
	{
		UNiagaraComponent* Effect = PathEffectComponents[i];
		if (IsValid(Effect))
		{
			// 나이아가라 시스템의 User 파라미터(User.BeamStart, User.BeamEnd)에 월드 좌표를 넣는다.
			Effect->SetVariableVec3(TEXT("BeamStart"), PathPoints[i] + Offset);
			Effect->SetVariableVec3(TEXT("BeamEnd"), PathPoints[i + 1] + Offset);
		}
	}
}

void UMoveComponent::SpawnGhostActor()
{
	if (IsValid(GhostActor))
	{
		return;
	}

	ABaseCharacter* BaseCharacter = GetOwner<ABaseCharacter>();
	if (!BaseCharacter)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = BaseCharacter->GetController();
	SpawnParams.Instigator = BaseCharacter;
	GhostActor = GetWorld()->SpawnActor<AGhostActor>(GhostActorClass, SpawnParams);
	if (GhostActor)
	{
		GhostActor->CopyMeshFrom(BaseCharacter->GetMesh());
	}
}

void UMoveComponent::DestroyGhostActor()
{
	if (IsValid(GhostActor))
	{
		GhostActor->Destroy();
	}
	GhostActor = nullptr;
}

void UMoveComponent::DestroyPathEffects()
{
	while (!PathEffectComponents.IsEmpty())
	{
		UNiagaraComponent* Effect = PathEffectComponents.Pop();
		if (IsValid(Effect))
		{
			Effect->DestroyComponent();
		}
	}
}

// ======================================================================
// 이동
// ======================================================================

void UMoveComponent::UpdateMove()
{
	APawn* Pawn = GetOwner<APawn>();
	if (!Pawn || !PathPoints.IsValidIndex(TargetPointIndex))
	{
		FinishMove();
		return;
	}

	// 지금 향하는 지점에 닿으면 다음 지점으로. 마지막 지점이면 이동 끝.
	const FVector PawnLocation = Pawn->GetActorLocation();
	if (FVector::Dist2D(PawnLocation, PathPoints[TargetPointIndex]) <= AcceptanceRadius)
	{
		++TargetPointIndex;
		if (TargetPointIndex >= PathPoints.Num())
		{
			FinishMove();
			return;
		}
	}

	// AddMovementInput은 한 프레임짜리 입력이라 매 틱 넣어야 한다.
	const FVector Direction = (PathPoints[TargetPointIndex] - PawnLocation).GetSafeNormal2D();
	Pawn->AddMovementInput(Direction);
}

void UMoveComponent::FinishMove()
{
	SetComponentTickEnabled(false);
	TickFunction.Unbind();

	bHasDestination = false;
	DestroyPathEffects();
	PathPoints.Empty();
	TargetPointIndex = 0;

	SetOwnerNavObstacle(true);

	// 도착하면 정해 둔 공격 대상을 공격한다. (대상이 없으면 서버에서 무시)
	if (ABaseCharacter* BaseCharacter = GetOwner<ABaseCharacter>())
	{
		BaseCharacter->ServerAttack();
	}
}

// ======================================================================
// 내비게이션
// ======================================================================

void UMoveComponent::SetOwnerNavObstacle(bool bEnable)
{
	if (ABaseCharacter* BaseCharacter = GetOwner<ABaseCharacter>())
	{
		BaseCharacter->GetCapsuleComponent()->SetCanEverAffectNavigation(bEnable);
	}
}
