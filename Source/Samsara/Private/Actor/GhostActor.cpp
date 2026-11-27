// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GhostActor.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Materials/MaterialInterface.h"

#include "LogTest.h"
#include "Component/RangeComponent.h"

AGhostActor::AGhostActor()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

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

	RangeComponent->Active();
}

void AGhostActor::CopyMeshFrom(const USkeletalMeshComponent* SourceMesh)
{
	if (!SourceMesh)
	{
		return;
	}

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

void AGhostActor::SetRange(float _Range)
{
	RangeComponent->SetSphereRadius(_Range);
}
