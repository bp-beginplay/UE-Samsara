// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SkillRange.h"

#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Character/BaseCharacter.h"

ASkillRange::ASkillRange()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// 데칼은 로컬 X축 방향으로 투영되므로 아래를 향하게 돌린다.
	SkillRangeDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("SkillRangeDecal"));
	SkillRangeDecal->SetupAttachment(Root);
	SkillRangeDecal->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
}

void ASkillRange::BeginPlay()
{
	Super::BeginPlay();

	const ABaseCharacter* BaseCharacter = GetOwner<ABaseCharacter>();
	if (!BaseCharacter)
	{
		return;
	}

	// X = 투영 깊이, Y/Z = 바닥에서의 반지름
	const float MaxWalkSpeed = BaseCharacter->GetCharacterMovement()->MaxWalkSpeed;
	SkillRangeDecal->DecalSize = FVector(100.f, MaxWalkSpeed, MaxWalkSpeed);
	SkillRangeDecal->MarkRenderStateDirty();
}
