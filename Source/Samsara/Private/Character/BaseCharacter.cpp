// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavAreas/NavArea_Null.h"
#include "Net/UnrealNetwork.h"

#include "Actor/GhostActor.h"
#include "Component/MoveComponent.h"
#include "Component/MoveRangeComponent.h"
#include "Component/RangeComponent.h"
#include "Data/Input/DA_ArhatInput.h"
#include "LogTest.h"
#include "PlayerController/PvPPlayerController.h"

// ======================================================================
// 기본 함수
// ======================================================================

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 카메라: 캐릭터 위에서 60도로 내려다보는 탑다운 시점. 캐릭터가 돌아도 카메라는 돌지 않는다.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 1000.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// 회전: 컨트롤러가 아니라 이동 방향으로 몸을 돌린다.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);

	// 바닥 데칼(사거리, 이동 범위)이 캐릭터 몸에 찍히지 않게 한다.
	GetMesh()->SetReceivesDecals(false);

	// 이동, 사거리
	MoveComponent = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComponent"));

	MoveRangeComponent = CreateDefaultSubobject<UMoveRangeComponent>(TEXT("MoveRangeComponent"));
	MoveRangeComponent->SetupAttachment(RootComponent);

	RangeComponent = CreateDefaultSubobject<URangeComponent>(TEXT("RangeComponent"));
	RangeComponent->SetupAttachment(RootComponent);

	// 내비게이션: 캡슐을 "지나갈 수 없는 영역"으로 등록해 다른 캐릭터의 경로가 이 캐릭터를 피해 가게 한다.
	// ACharacter는 시작할 때 캡슐의 CanEverAffectNavigation을 bCanAffectNavigationGeneration 값으로 덮어쓰므로 둘 다 켠다.
	bCanAffectNavigationGeneration = true;
	GetCapsuleComponent()->SetCanEverAffectNavigation(true);
	GetCapsuleComponent()->bDynamicObstacle = true;
	GetCapsuleComponent()->SetAreaClassOverride(UNavArea_Null::StaticClass());
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, Team);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraOffset(DeltaTime);
}

void ABaseCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	// 빙의할 때 카메라를 캐릭터 위치로 되돌린다.
	SpringArm->TargetOffset = FVector::ZeroVector;
	CameraTargetOffset = FVector::ZeroVector;
}

void ABaseCharacter::UnPossessed()
{
	ClientOnUnpossessed();

	Super::UnPossessed();
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!InputData)
	{
		return;
	}

	InputData->AddMapping(GetController());
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		InputBinding::BindTriggered(Input, InputData->GetCameraMoveMouseAction(), this, &ABaseCharacter::OnCameraMove);
		InputBinding::BindStarted(Input, InputData->GetSelectDestinationAction(), this, &ABaseCharacter::OnSelectDestination);
		InputBinding::BindCompleted(Input, InputData->GetConfirmDestinationAction(), this, &ABaseCharacter::OnConfirmDestination);
		InputBinding::BindStarted(Input, InputData->GetStartMoveAction(), this, &ABaseCharacter::OnStartMove);
		InputBinding::BindStarted(Input, InputData->GetShowAttackRangeAction(), this, &ABaseCharacter::OnShowAttackRange);
	}
}

// ======================================================================
// 입력 처리
// ======================================================================

void ABaseCharacter::OnCameraMove(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();
	CameraTargetOffset += FVector(Input.Y, Input.X, 0.f) * CameraSpeed;
}

void ABaseCharacter::OnSelectDestination()
{
	// 목적지 고르기 시작: 이동 미리보기와 실제 이동 가능 범위를 보여 준다.
	MoveComponent->BeginSelectDestination();
	MoveRangeComponent->Show();
}

void ABaseCharacter::OnConfirmDestination()
{
	if (MoveComponent->ConfirmDestination())
	{
		// 목적지가 정해지면 이동 가능 범위 표시는 숨긴다.
		MoveRangeComponent->Hide();
		LOG_TEXT("Success ConfirmDestination");
	}
	else
	{
		// 목적지를 고르는 중이 아니면 커서 아래 캐릭터를 공격 대상으로 정한다.
		// 목적지를 확정했으면 고스트(도착 위치) 사거리, 아니면 지금 위치 사거리 안에 있어야 한다.
		const APvPPlayerController* PC = Cast<APvPPlayerController>(GetController());
		if (!PC)
		{
			return;
		}
		const FHitResult Hit = PC->GetUnderCursorHit();
		if (ABaseCharacter* Target = Cast<ABaseCharacter>(Hit.GetActor()))
		{
			TArray<TObjectPtr<ABaseCharacter>> TargetsInRange;
			AGhostActor* GhostActor = MoveComponent->GetGhostActor();
			if (IsValid(GhostActor))
			{
				TargetsInRange = GhostActor->GetTargetsInRange();
			}
			else
			{
				TargetsInRange = RangeComponent->GetTargetsInRange();
			}
			if (TargetsInRange.Contains(Target))
			{
				ServerSetAttackTarget(Target);
			}
			else
			{
				LOG_TEXT("Fail Target");
			}
		}
	}
}

void ABaseCharacter::OnStartMove()
{
	if (MoveComponent->StartMove())
	{
		LOG_TEXT("Success StartMove");
	}
	else
	{
		// 확정한 목적지가 없으면 제자리에서 공격한다.
		ServerAttack();
	}
}

void ABaseCharacter::OnShowAttackRange()
{
	// 목적지를 정했으면 고스트(도착 위치) 기준, 아니면 캐릭터 기준으로 공격 사거리를 표시한다.
	AGhostActor* GhostActor = MoveComponent->GetGhostActor();
	if (IsValid(GhostActor))
	{
		GhostActor->SetRangeRadius(AttackRange);
	}
	else
	{
		RangeComponent->SetRangeRadius(AttackRange);
		RangeComponent->EnableDetection();
	}
}

// ======================================================================
// 카메라
// ======================================================================

void ABaseCharacter::UpdateCameraOffset(float DeltaTime)
{
	SpringArm->TargetOffset = UKismetMathLibrary::VInterpTo(SpringArm->TargetOffset, CameraTargetOffset, DeltaTime, CameraSmoothing);
}

// ======================================================================
// RPC
// ======================================================================

void ABaseCharacter::ClientOnUnpossessed_Implementation()
{
	// 빙의가 풀리면 이동 가능 범위 표시를 숨긴다.
	MoveRangeComponent->Hide();
}

void ABaseCharacter::ServerSetAttackTarget_Implementation(ABaseCharacter* NewTarget)
{
	SetAttackTarget(NewTarget);
}

void ABaseCharacter::MulticastPlayAttack_Implementation(ABaseCharacter* Target)
{
	// 대상 쪽으로 몸을 돌린다. (높이 차이는 무시하고 평면 방향만)
	if (IsValid(Target))
	{
		const FVector Direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		if (!Direction.IsNearlyZero())
		{
			SetActorRotation(Direction.Rotation());
		}
	}

	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}
}

void ABaseCharacter::ServerAttack_Implementation()
{
	// 대상이 없거나 이미 사라졌으면 공격하지 않는다.
	if (!IsValid(AttackTarget))
	{
		return;
	}

	MulticastPlayAttack(AttackTarget);
}
