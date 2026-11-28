// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interface/TeamInterface.h"

#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UTexture2D;
class UDA_ArhatInput;
class URangeComponent;
class UMoveComponent;
class UMoveRangeComponent;
class UAnimMontage;
struct FInputActionValue;

/** UI에 쓰는 아라한 이미지 묶음. */
USTRUCT()
struct FArhatIcons
{
	GENERATED_BODY()

	/** 아라한 선택 버튼에 표시하는 아이콘. */
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* ArhatIcon = nullptr;
};

/**
 * 플레이어가 조종하는 아라한(캐릭터)의 공통 클래스.
 *
 * 입력
 *  - 마우스 이동 : 카메라 이동
 *  - S           : 목적지 고르기 시작 (이동 미리보기 + 이동 가능 범위 표시)
 *  - 좌클릭      : 목적지 확정. 목적지를 고르는 중이 아니면 사거리 안의 캐릭터를 공격 대상으로 정한다.
 *  - 스페이스    : 확정한 목적지로 이동하고, 도착하면 공격 대상을 공격한다. 목적지가 없으면 제자리에서 공격한다.
 *  - A           : 공격 사거리 표시
 *
 * 공격
 *  - 대상 설정(ServerSetAttackTarget)과 공격(ServerAttack)은 서버에서 하고,
 *    공격 애니메이션(MulticastPlayAttack)은 모든 플레이어 화면에서 재생한다.
 */
UCLASS()
class SAMSARA_API ABaseCharacter : public ACharacter, public ITeamInterface
{
	GENERATED_BODY()

	// 기본 함수
public:
	/** 카메라, 이동·사거리 컴포넌트를 만들고 캡슐을 내비게이션 장애물로 등록한다. */
	ABaseCharacter();
	/** Team을 복제한다. */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/** 카메라 오프셋을 갱신한다. */
	virtual void Tick(float DeltaTime) override;
	/** 빙의할 때(클라이언트) 카메라를 캐릭터 위치로 되돌린다. */
	virtual void PawnClientRestart() override;
	/** 빙의가 풀릴 때(서버) 클라이언트에 정리를 요청한다. */
	virtual void UnPossessed() override;
protected:
	/** 입력 매핑을 추가하고 입력 액션을 아래 처리 함수에 연결한다. */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 커스텀 함수
public:
	/** (서버) 서버에 정해 둔 AttackTarget을 공격한다. 모든 플레이어 화면에 공격 애니메이션이 재생된다. 조종하는 클라이언트에서 호출한다. (MoveComponent가 이동이 끝나면 호출) */
	UFUNCTION(Reliable, Server)
	void ServerAttack();

private:
	// 입력 처리
	/** 마우스 이동량만큼 카메라 목표 위치를 옮긴다. */
	void OnCameraMove(const FInputActionValue& Value);
	/** S: 목적지 고르기를 시작하고 이동 가능 범위를 표시한다. */
	void OnSelectDestination();
	/** 좌클릭: 목적지를 확정하고 이동 가능 범위를 숨긴다. 목적지를 고르는 중이 아니면 커서 아래 캐릭터를 공격 대상으로 정한다. */
	void OnConfirmDestination();
	/** 스페이스: 확정한 목적지로 이동을 시작한다. 목적지가 없으면 제자리에서 공격한다. */
	void OnStartMove();
	/** A: 고스트가 있으면 고스트 기준, 없으면 캐릭터 기준으로 공격 사거리를 표시한다. */
	void OnShowAttackRange();

	// 카메라
	/** 카메라 오프셋을 목표 위치(CameraTargetOffset)로 부드럽게 옮긴다. */
	void UpdateCameraOffset(float DeltaTime);

	// 공격
	/** 커서 아래 캐릭터가 지금 공격 사거리 안에 있으면 서버에 공격 대상으로 정해 달라고 요청한다. */
	void SelectAttackTargetUnderCursor();
	/** 지금 공격 사거리 안의 캐릭터들. 목적지를 확정했으면 고스트(도착 위치) 기준, 아니면 캐릭터 기준. */
	const TArray<TObjectPtr<ABaseCharacter>>& GetTargetsInAttackRange() const;
	/** 대상 쪽으로 몸을 돌린다. (높이 차이는 무시하고 평면 방향만, 즉시) */
	void FaceTarget(const AActor* Target);

	// RPC
	/** 빙의가 풀릴 때 조종하던 클라이언트에서 표시들을 정리한다. */
	UFUNCTION(Reliable, Client)
	void ClientOnUnpossessed();
	/** (서버) 공격할 대상을 정한다. 조종하는 클라이언트에서 호출한다. nullptr이면 대상을 지운다. */
	UFUNCTION(Reliable, Server)
	void ServerSetAttackTarget(ABaseCharacter* NewTarget);
	/** (서버에서 호출 → 모든 머신에서 실행) 대상 쪽으로 몸을 돌리고 공격 몽타주를 재생한다. Target이 nullptr이면 방향은 그대로 둔다. */
	UFUNCTION(Reliable, NetMulticast)
	void MulticastPlayAttack(ABaseCharacter* Target);

	// 변수
private:
	// 카메라
	/** 위에서 60도로 내려다보는 카메라 붐. 캐릭터가 돌아도 돌지 않는다. */
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	/** 탑다운 카메라. */
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;
	/** 카메라가 목표 위치를 따라가는 속도. 클수록 빨리 따라간다. */
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraSmoothing = 2.f;
	/** 마우스 이동량 1당 카메라 목표 위치가 움직이는 거리. */
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraSpeed = 30.0f;
	/** 카메라 오프셋의 목표 위치. 입력으로 바뀌고 Tick에서 부드럽게 따라간다. */
	FVector CameraTargetOffset;

	// 이동, 사거리
	/** 경로 미리보기, 목적지 확정, 경로 따라 이동. */
	UPROPERTY(VisibleAnywhere, Category = "Move")
	TObjectPtr<UMoveComponent> MoveComponent;
	/** 목적지를 고르는 동안 실제 이동 가능 범위를 바닥에 표시하는 데칼. */
	UPROPERTY(VisibleAnywhere, Category = "Move")
	TObjectPtr<UMoveRangeComponent> MoveRangeComponent;
	/** 캐릭터 위치 기준 공격 사거리. */
	UPROPERTY(VisibleAnywhere, Category = "Range")
	TObjectPtr<URangeComponent> RangeComponent;
	/** A 키로 표시하는 공격 사거리. */
	UPROPERTY(EditDefaultsOnly, Category = "Range", meta = (ClampMin = "0.0"))
	float AttackRange = 600.f;
	/** 공격할 대상. 없으면 nullptr. */
	UPROPERTY(VisibleInstanceOnly, Transient, Category = "Attack")
	TObjectPtr<ABaseCharacter> AttackTarget;
	/** 기본 공격 몽타주. 캐릭터 블루프린트에서 지정한다. */
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackMontage;

	// 데이터
	/** 아라한 선택 버튼 등 UI에 쓰는 이미지. */
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	FArhatIcons ArhatIcons;
	/** 이 캐릭터를 조종할 때 쓰는 입력 매핑과 입력 액션. */
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDA_ArhatInput* InputData;
	/** 소속 팀. 스폰할 때 게임모드가 정하고 모든 클라이언트에 복제한다. */
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Team")
	ETeam Team = ETeam::None;

	// Get/Set 함수
public:
	/** UI에 쓰는 아라한 이미지. */
	FArhatIcons GetArhatIcons() const { return ArhatIcons; }
	/** 공격할 대상. 없으면 nullptr. */
	ABaseCharacter* GetAttackTarget() const { return AttackTarget; }
	/** 공격할 대상을 정한다. nullptr을 넣으면 대상을 지운다. */
	void SetAttackTarget(ABaseCharacter* NewTarget) { AttackTarget = NewTarget; }

	// ITeamInterface
	virtual ETeam GetTeam() const override { return Team; }
	virtual void SetTeam(ETeam NewTeam) override { Team = NewTeam; }
};
