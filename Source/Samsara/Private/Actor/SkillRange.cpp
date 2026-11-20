

#include "Actor/SkillRange.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Character/BaseCharacter.h"
#include "LogTest.h"

ASkillRange::ASkillRange()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	SkillRangeDecal = CreateDefaultSubobject<UDecalComponent>("SkillRangeDecal");
	SkillRangeDecal->SetupAttachment(Root);
	SkillRangeDecal->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));


}

void ASkillRange::BeginPlay()
{
	Super::BeginPlay();

	ABaseCharacter* BaseCharacter = GetOwner<ABaseCharacter>();
	const float MaxWalkSpeed = BaseCharacter->GetCharacterMovement()->MaxWalkSpeed;
	SkillRangeDecal->DecalSize = FVector(100.f, MaxWalkSpeed, MaxWalkSpeed);
	SkillRangeDecal->MarkRenderStateDirty();
}