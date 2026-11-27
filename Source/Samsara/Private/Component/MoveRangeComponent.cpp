// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MoveRangeComponent.h"

#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PawnMovementComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "TimerManager.h"
#include "Engine/World.h"

UMoveRangeComponent::UMoveRangeComponent()
{
	// 턴제라 한 번에 계산하므로 틱은 쓰지 않는다.
	PrimaryComponentTick.bCanEverTick = false;

	// 데칼은 로컬 X축 방향으로 투영되므로 아래를 향하게 돌린다.
	// 캐릭터가 회전해도 바닥 기준 방향이 바뀌지 않도록 회전은 절대값으로 둔다.
	// 이 회전에서 데칼 UV는 U = 월드 X, V = 월드 Y 방향이 된다. (DeferredDecal.usf: UV = 로컬 (Z, Y))
	SetUsingAbsoluteRotation(true);
	SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

	SetVisibility(false);
}

void UMoveRangeComponent::Show()
{
	const APawn* Pawn = GetOwner<APawn>();
	if (!Pawn)
	{
		return;
	}

	// 반지름 = 한 턴에 이동할 수 있는 거리 (MoveComponent의 이동 제한과 같은 값)
	Radius = 0.f;
	if (const UPawnMovementComponent* Movement = Pawn->GetMovementComponent())
	{
		Radius = Movement->GetMaxSpeed();
	}
	if (Radius <= 0.f)
	{
		return;
	}
	Center = Pawn->GetActorLocation();

	// X = 투영 깊이, Y/Z = 바닥에서의 반지름
	DecalSize = FVector(ProjectionDepth, Radius, Radius);
	MarkRenderStateDirty();

	// 계산 전에는 빈 마스크(아무것도 안 보임)로 시작한다.
	PrepareMask();
	SetVisibility(true);

	// 자기 캡슐의 내비게이션 장애물이 꺼진 것이 반영된 뒤 한 번에 계산한다.
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ComputeTimer);
		if (StartDelay > 0.f)
		{
			World->GetTimerManager().SetTimer(ComputeTimer, this, &UMoveRangeComponent::ComputeRange, StartDelay, false);
		}
		else
		{
			ComputeRange();
		}
	}
}

void UMoveRangeComponent::Hide()
{
	// 아직 계산 전이면 취소한다.
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ComputeTimer);
	}
	SetVisibility(false);
}

void UMoveRangeComponent::ComputeRange()
{
	const double StartTime = FPlatformTime::Seconds();

	for (int32 Y = 0; Y < GridResolution; ++Y)
	{
		for (int32 X = 0; X < GridResolution; ++X)
		{
			Reachable[Y * GridResolution + X] = IsCellReachable(X, Y) ? 1.f : 0.f;
		}
	}

	RebuildMaskPixels();
	UploadMask();

	// 계산 시간 확인용. 너무 오래 걸리면 GridResolution을 낮춘다.
	UE_LOG(LogTemp, Log, TEXT("MoveRange: %d x %d cells computed in %.2f ms"),
		GridResolution, GridResolution, (FPlatformTime::Seconds() - StartTime) * 1000.0);
}

void UMoveRangeComponent::PrepareMask()
{
	// 텍스처는 한 번 만들어 재사용한다. 크기가 바뀌었으면 다시 만든다.
	if (!MaskTexture || MaskTexture->GetSizeX() != TextureResolution)
	{
		MaskTexture = UTexture2D::CreateTransient(TextureResolution, TextureResolution, PF_B8G8R8A8);
		MaskTexture->SRGB = false;
		MaskTexture->Filter = TF_Bilinear;
		MaskTexture->AddressX = TA_Clamp;
		MaskTexture->AddressY = TA_Clamp;
		MaskTexture->UpdateResource();
	}

	// 모든 칸을 "이동 불가"로 시작한다.
	Reachable.Init(0.f, GridResolution * GridResolution);
	MaskPixels.SetNumUninitialized(TextureResolution * TextureResolution * 4);
	RebuildMaskPixels();
	UploadMask();

	// 데칼 머티리얼로 동적 인스턴스를 만들어 마스크 텍스처를 넘긴다.
	UMaterialInterface* BaseMaterial = GetDecalMaterial();
	if (BaseMaterial && (!MaskMaterial || MaskMaterial->Parent != BaseMaterial))
	{
		MaskMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		SetDecalMaterial(MaskMaterial);
	}
	if (MaskMaterial)
	{
		MaskMaterial->SetTextureParameterValue(MaskParameterName, MaskTexture);
	}
}

bool UMoveRangeComponent::IsCellReachable(int32 X, int32 Y) const
{
	// 칸 중심의 월드 위치. 칸 (0,0)이 (Center.X - Radius, Center.Y - Radius) 쪽 모서리.
	const float CellSize = (Radius * 2.f) / GridResolution;
	FVector Target(
		Center.X - Radius + (X + 0.5f) * CellSize,
		Center.Y - Radius + (Y + 0.5f) * CellSize,
		Center.Z);

	// 원에서 충분히 먼 칸은 바로 제외한다.
	// 원 바깥 1.5칸까지는 일부러 검사한다. 칸 결과는 보간되어 경계가 칸 사이에 생기는데,
	// 원 바로 바깥 칸까지 "이동 가능"으로 채워 두면 그 경계가 원 밖으로 밀려나고,
	// 실제 바깥 모양은 RebuildMaskPixels의 픽셀 단위 원이 정하게 되어 칸 모양이 드러나지 않는다.
	const float Dist = FVector::Dist2D(Center, Target);
	if (Dist > Radius + CellSize * 1.5f)
	{
		return false;
	}
	// 원 밖의 칸은 같은 방향의 원 안쪽 지점으로 검사한다. (그 방향으로 원 끝까지 갈 수 있는지)
	if (Dist > Radius)
	{
		const FVector Dir = (Target - Center).GetSafeNormal2D();
		Target = FVector(Center.X + Dir.X * Radius * 0.99f, Center.Y + Dir.Y * Radius * 0.99f, Center.Z);
	}

	AActor* Owner = GetOwner();
	UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(
		const_cast<UMoveRangeComponent*>(this), Center, Target, Owner);
	if (!Path || !Path->IsValid() || Path->IsPartial() || Path->PathPoints.Num() < 2)
	{
		return false;
	}

	// 칸이 벽이나 다른 캐릭터 안이면 길찾기가 가까운 내비메시 위치로 끝점을 옮긴다.
	// 그 경우는 칸 자체에는 설 수 없으므로 이동 불가로 본다.
	if (FVector::Dist2D(Path->PathPoints.Last(), Target) > CellSize * 0.75f)
	{
		return false;
	}

	// MoveComponent와 같은 기준: PathPoints 구간 길이 합이 이동 거리 이내
	float Length = 0.f;
	for (int32 i = 1; i < Path->PathPoints.Num(); ++i)
	{
		Length += FVector::Dist(Path->PathPoints[i - 1], Path->PathPoints[i]);
	}
	return Length <= Radius;
}

float UMoveRangeComponent::SampleReachable(float U, float V) const
{
	// 칸 중심 기준 좌표로 바꾼 뒤, 주변 4칸을 거리 비율로 섞는다. (bilinear)
	const float GX = U * GridResolution - 0.5f;
	const float GY = V * GridResolution - 0.5f;
	const int32 X0 = FMath::Clamp(FMath::FloorToInt(GX), 0, GridResolution - 1);
	const int32 Y0 = FMath::Clamp(FMath::FloorToInt(GY), 0, GridResolution - 1);
	const int32 X1 = FMath::Min(X0 + 1, GridResolution - 1);
	const int32 Y1 = FMath::Min(Y0 + 1, GridResolution - 1);
	const float TX = FMath::Clamp(GX - X0, 0.f, 1.f);
	const float TY = FMath::Clamp(GY - Y0, 0.f, 1.f);

	const float A = Reachable[Y0 * GridResolution + X0];
	const float B = Reachable[Y0 * GridResolution + X1];
	const float C = Reachable[Y1 * GridResolution + X0];
	const float D = Reachable[Y1 * GridResolution + X1];
	return FMath::Lerp(FMath::Lerp(A, B, TX), FMath::Lerp(C, D, TX), TY);
}

void UMoveRangeComponent::RebuildMaskPixels()
{
	const float PixelWorldSize = (Radius * 2.f) / TextureResolution;

	for (int32 PY = 0; PY < TextureResolution; ++PY)
	{
		for (int32 PX = 0; PX < TextureResolution; ++PX)
		{
			// 픽셀 중심의 UV와, 데칼 중심 기준 월드 거리. (U = 월드 X, V = 월드 Y)
			const float U = (PX + 0.5f) / TextureResolution;
			const float V = (PY + 0.5f) / TextureResolution;
			const float OffsetX = (U * 2.f - 1.f) * Radius;
			const float OffsetY = (V * 2.f - 1.f) * Radius;
			const float Dist = FMath::Sqrt(OffsetX * OffsetX + OffsetY * OffsetY);

			// 바깥 원: 픽셀 단위로 정확히 자르고, 경계 1픽셀은 부드럽게(안티에일리어싱)
			const float Circle = FMath::Clamp((Radius - Dist) / PixelWorldSize + 0.5f, 0.f, 1.f);

			// 장애물 경계: 칸 결과를 주변까지 섞어 흐리게 만든 뒤(모서리가 둥글어짐) 0.5 근처에서 다시 또렷하게 만든다.
			// 흐리게만 하면 경계가 뿌옇고, 그대로 쓰면 계단 모양이라 둘을 섞는다.
			// 가운데 + 상하좌우 4곳을 섞는 간단한 블러. EdgeRoundness가 클수록 모서리가 더 둥글다.
			const float Blur = EdgeRoundness / GridResolution;
			const float Smooth =
				SampleReachable(U, V) * 0.4f +
				(SampleReachable(U + Blur, V) + SampleReachable(U - Blur, V) +
				 SampleReachable(U, V + Blur) + SampleReachable(U, V - Blur)) * 0.15f;
			const float Edge = FMath::SmoothStep(0.4f, 0.6f, Smooth);

			const uint8 Value = static_cast<uint8>(FMath::RoundToInt(Circle * Edge * 255.f));
			const int32 Index = (PY * TextureResolution + PX) * 4;
			MaskPixels[Index + 0] = Value;
			MaskPixels[Index + 1] = Value;
			MaskPixels[Index + 2] = Value;
			MaskPixels[Index + 3] = 255;
		}
	}
}

void UMoveRangeComponent::UploadMask()
{
	if (!MaskTexture || !MaskTexture->GetPlatformData() || MaskTexture->GetPlatformData()->Mips.IsEmpty())
	{
		return;
	}

	FTexture2DMipMap& Mip = MaskTexture->GetPlatformData()->Mips[0];
	void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(Data, MaskPixels.GetData(), MaskPixels.Num());
	Mip.BulkData.Unlock();
	MaskTexture->UpdateResource();
}
