// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GhostActor.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Materials/MaterialInterface.h"

#include "Component/RangeComponent.h"

AGhostActor::AGhostActor()
{
	// 틱에서 할 일이 없다.
	PrimaryActorTick.bCanEverTick = false;

	// 조종하는 플레이어 화면에만 필요한 미리보기라 복제하지 않는다.
	bReplicates = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// 충돌을 켜 두면 실제 캐릭터가 고스트에 막히고 커서 판정도 가로막는다.
	GhostMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GhostMesh"));
	GhostMesh->SetupAttachment(Root);
	GhostMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GhostMesh->SetGenerateOverlapEvents(false);
	GhostMesh->SetCastShadow(false);
	GhostMesh->SetReceivesDecals(false);

	RangeComponent = CreateDefaultSubobject<URangeComponent>(TEXT("RangeComponent"));
	RangeComponent->SetupAttachment(RootComponent);
	RangeComponent->bHiddenInGame = false;
}

void AGhostActor::BeginPlay()
{
	Super::BeginPlay();

	// 고스트는 조종하는 플레이어 화면에만 스폰되므로 바로 사거리를 켠다.
	RangeComponent->EnableDetection();
}

void AGhostActor::CopyMeshFrom(const USkeletalMeshComponent* SourceMesh)
{
	if (!SourceMesh)
	{
		return;
	}

	// 캐릭터 메시는 보통 캡슐 기준 Yaw -90 회전되어 있어 회전과 스케일을 맞춘다.
	// 위치는 복사하지 않는다. 고스트는 바닥 지점에 스폰하므로 메시 원점(발)이 그 지점에 오면 된다.
	GhostMesh->SetSkeletalMeshAsset(SourceMesh->GetSkeletalMeshAsset());
	GhostMesh->SetRelativeRotation(SourceMesh->GetRelativeRotation());
	GhostMesh->SetRelativeScale3D(SourceMesh->GetRelativeScale3D());

	if (GhostMaterial)
	{
		for (int32 i = 0; i < GhostMesh->GetNumMaterials(); ++i)
		{
			GhostMesh->SetMaterial(i, GhostMaterial);
		}
	}
}

void AGhostActor::SetRangeRadius(float InRadius)
{
	RangeComponent->SetRangeRadius(InRadius);
}
