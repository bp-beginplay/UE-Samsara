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
struct FInputActionValue;

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
 *  - 좌클릭      : 목적지 확정
 *  - 스페이스    : 확정한 목적지로 이동
 *  - A           : 공격 사거리 표시
 */
UCLASS()
class SAMSARA_API ABaseCharacter : public ACharacter, public ITeamInterface
{
	GENERATED_BODY()

	// 기본 함수
public:
	ABaseCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;
	virtual void PawnClientRestart() override;
	virtual void UnPossessed() override;
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 커스텀 함수
private:
	// 입력 처리
	void OnCameraMove(const FInputActionValue& Value);
	void OnSelectDestination();
	void OnConfirmDestination();
	void OnStartMove();
	void OnShowAttackRange();

	/** 카메라 오프셋을 목표 위치(CameraTargetOffset)로 부드럽게 옮긴다. */
	void UpdateCameraOffset(float DeltaTime);

	/** 빙의가 풀릴 때 조종하던 클라이언트에서 표시들을 정리한다. */
	UFUNCTION(Reliable, Client)
	void ClientOnUnpossessed();

	// 변수
private:
	// 카메라
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraSmoothing = 2.f;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraSpeed = 30.0f;
	/** 카메라 오프셋의 목표 위치. 입력으로 바뀌고 Tick에서 부드럽게 따라간다. */
	FVector CameraTargetOffset;

	// 이동, 사거리
	UPROPERTY(VisibleAnywhere, Category = "Move")
	TObjectPtr<UMoveComponent> MoveComponent;
	/** 목적지를 고르는 동안 실제 이동 가능 범위를 바닥에 표시하는 데칼. */
	UPROPERTY(VisibleAnywhere, Category = "Move")
	TObjectPtr<UMoveRangeComponent> MoveRangeComponent;
	UPROPERTY(VisibleAnywhere, Category = "Range")
	TObjectPtr<URangeComponent> RangeComponent;
	/** A 키로 표시하는 공격 사거리. */
	UPROPERTY(EditDefaultsOnly, Category = "Range", meta = (ClampMin = "0.0"))
	float AttackRange = 600.f;

	// 데이터
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	FArhatIcons ArhatIcons;
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDA_ArhatInput* InputData;
	UPROPERTY(VisibleAnywhere, Replicated)
	int32 TeamId = INDEX_NONE;

	// Get/Set 함수
public:
	FArhatIcons GetArhatIcons() const { return ArhatIcons; }

	// ITeamInterface
	virtual int32 GetTeamId() const override { return TeamId; }
	virtual void SetTeamId(int32 NewTeamId) override { TeamId = NewTeamId; }
};
