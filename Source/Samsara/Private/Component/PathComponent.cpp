// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PathComponent.h"

#include "Components/SplineMeshComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"

UPathComponent::UPathComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	SetUsingAbsoluteLocation(true);
	SetUsingAbsoluteRotation(true);
	SetUsingAbsoluteScale(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultPathMesh(TEXT("/Engine/BasicShapes/Plane.Plane"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DefaultPathMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (DefaultPathMesh.Succeeded())
		PathMesh = DefaultPathMesh.Object;
	if (DefaultPathMaterial.Succeeded())
		PathMaterial = DefaultPathMaterial.Object;
}

void UPathComponent::UpdatePathVisual(float CurDist)
{
	const float TotalLength = GetSplineLength();
	const float Remaining = FMath::Max(TotalLength - CurDist, 0.f);
	const int32 NumSegments = FMath::CeilToInt(Remaining / SegmentLength);
	while (PathSegments.Num() < NumSegments)
	{
		USplineMeshComponent* Segment = NewObject<USplineMeshComponent>(GetOwner());
		Segment->SetMobility(EComponentMobility::Movable);
		Segment->SetUsingAbsoluteLocation(true);
		Segment->SetUsingAbsoluteRotation(true);
		Segment->SetUsingAbsoluteScale(true);
		Segment->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Segment->SetCastShadow(false);
		Segment->SetForwardAxis(ESplineMeshAxis::X, false);
		Segment->SetStaticMesh(PathMesh);
		if (PathMaterial)
		{
			Segment->SetMaterial(0, PathMaterial);
		}
		Segment->SetStartScale(FVector2D(PathWidthScale, 1.f), false);
		Segment->SetEndScale(FVector2D(PathWidthScale, 1.f), false);
		Segment->SetupAttachment(this);
		Segment->RegisterComponent();
		PathSegments.Add(Segment);
	}
	const FVector Offset(0.f, 0.f, PathHeightOffset);
	for (int32 i = 0; i < PathSegments.Num(); ++i)
	{
		USplineMeshComponent* Segment = PathSegments[i];
		if (i >= NumSegments)
		{
			Segment->SetVisibility(false);
			continue;
		}
		const float StartDist = CurDist + i * SegmentLength;
		const float EndDist = FMath::Min(StartDist + SegmentLength, TotalLength);
		const float Length = EndDist - StartDist;
		const FVector StartPos = GetLocationAtDistanceAlongSpline(StartDist, ESplineCoordinateSpace::World) + Offset;
		const FVector EndPos = GetLocationAtDistanceAlongSpline(EndDist, ESplineCoordinateSpace::World) + Offset;
		const FVector StartTangent = GetDirectionAtDistanceAlongSpline(StartDist, ESplineCoordinateSpace::World) * Length;
		const FVector EndTangent = GetDirectionAtDistanceAlongSpline(EndDist, ESplineCoordinateSpace::World) * Length;
		Segment->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
		Segment->SetVisibility(true);
	}
}

void UPathComponent::BuildPathTo(const FVector& Destination)
{
	AActor* Owner = GetOwner();
	UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(this, Owner->GetActorLocation(), Destination, Owner);
	ClearSplinePoints(false);
	for (const FVector& Point : Path->PathPoints)
	{
		AddSplinePoint(Point, ESplineCoordinateSpace::World, false);
	}
	for (int32 i = 0; i < GetNumberOfSplinePoints(); ++i)
	{
		SetSplinePointType(i, ESplinePointType::Linear, false);
	}
	UpdateSpline();
}

void UPathComponent::HidePathVisual()
{
	for (USplineMeshComponent* Segment : PathSegments)
	{
		if (Segment)
		{
			Segment->SetVisibility(false);
		}
	}
}
