// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MoveComponent.h"

#include "Components/SplineMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Materials/MaterialInterface.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Character/BaseCharacter.h"
#include "PlayerController/PvPPlayerController.h"
#include "Component/RouteComponent.h"
#include "LogTest.h"
#include "Actor/GhostActor.h"

void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	RouteComponent = NewObject<URouteComponent>(GetOwner());
	RouteComponent->SetupAttachment(this);
	RouteComponent->RegisterComponent();
}

void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* Pawn = Cast<APawn>(GetOwner());
	UpdateAutoMove(DeltaTime, Pawn);
}

void UMoveComponent::Active()
{
	RouteComponent->Active();
}

void UMoveComponent::DeActive()
{
	RouteComponent->DeActive();
	SetComponentTickEnabled(false);
	if (IsValid(GhostActor))
		GhostActor->Destroy();
	HidePathVisual();
	ClearSplinePoints(true);
}

bool UMoveComponent::SetTarget()
{
	if (!RouteComponent->GetSetTarget())
		return false;
	
	FVector TargetLocation = RouteComponent->GetTargetLocation();

	if (IsValid(GhostActor))
		GhostActor->Destroy();
	ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
	APvPPlayerController* PC = Cast<APvPPlayerController>(Character->GetController());
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PC;
	SpawnParams.Instigator = Character;
	GhostActor = GetWorld()->SpawnActor<AGhostActor>(GhostActorClass, TargetLocation, FRotator::ZeroRotator, SpawnParams);
	GhostActor->CopyMeshFrom(Character->GetMesh());
	BuildPathTo(TargetLocation);
	UpdatePathVisual(0);

	return true;
}

bool UMoveComponent::Move()
{
	if (!IsValid(GhostActor))
		return false;

	GhostActor->Destroy();
	RouteComponent->DeActive();
	SetComponentTickEnabled(true);
	return true;
}

void UMoveComponent::UpdateAutoMove(float DeltaTime, APawn* Pawn)
{
	const float TotalLength = GetSplineLength();
	const float Key = FindInputKeyClosestToWorldLocation(Pawn->GetActorLocation());
	const float CurDist = GetDistanceAlongSplineAtSplineInputKey(Key);
	const float Remaining = TotalLength - CurDist;
	if (Remaining <= AcceptanceRadius)
	{
		StopMove();
		return;
	}
	const float TargetDist = FMath::Min(CurDist + LookAheadDistance, TotalLength);
	const FVector Target = GetLocationAtDistanceAlongSpline(TargetDist, ESplineCoordinateSpace::World);
	const FVector Direction = (Target - Pawn->GetActorLocation()).GetSafeNormal2D();
	const float InputScale = FMath::Clamp(Remaining / SlowDownDistance, 0.3f, 1.f);
	Pawn->AddMovementInput(Direction, InputScale);
	UpdatePathVisual(CurDist);
}

void UMoveComponent::StopMove()
{
	HidePathVisual();
	SetComponentTickEnabled(false);
}